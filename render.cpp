#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <vector>
#include "canvas.h"
#include "color.h"
#include "light.h"
#include "lodepng/lodepng.h"
#include "lua.hpp"
#include "lua_helpers.h"
#include "sphere.h"
#include "util.h"
#include "vec.h"

vec3f reflect(const vec3f& direction, const vec3f& normal)
{
    // Both `direction` and `normal` should be normalized.
    const float cosine{-(normal * direction)};
    return direction + 2.f * cosine * normal;
}

vec3f refract(
    const vec3f& direction, const vec3f& normal, const float refractive_index
) {
    // Both `direction` and `normal` should be normalized.
    float cosine_incidence{-(normal * direction)};
    float relative_index;
    vec3f new_normal;
    if (cosine_incidence < 0.f) {
        // Ray is inside the object.
        cosine_incidence = -cosine_incidence;
        relative_index = refractive_index;
        new_normal = -normal;
    } else {
        relative_index = 1.f / refractive_index;
        new_normal = normal;
    }

    const float cosine_sq_refract{
        1 - square(relative_index) * (1 - square(cosine_incidence))
    };
    if (cosine_sq_refract < 0.f) {
        return vec3f::zero;
    }
    const float cosine_refract{sqrtf(cosine_sq_refract)};
    return relative_index * direction
        + (relative_index * cosine_incidence - cosine_refract) * new_normal;
}

vec3f perturb(const vec3f& point, const vec3f& direction, const vec3f& normal)
{
    const float k{same_direction(direction, normal) ? 1e-3f : -1e-3f};
    return k * normal + point;
}

color cast_ray(
    const vec3f& origin,
    const vec3f& direction,
    const std::vector<sphere>& spheres,
    const std::vector<light>& lights,
    const int recursion_depth = 0
) {
    // Find the closest sphere that the ray intersects with and update `hit_point`
    auto result{std::end(spheres)};
    vec3f hit_point;
    for (auto it = std::begin(spheres); it != std::end(spheres); ++it) {
        if (const auto opt{it->ray_hit_point(origin, direction)}) {
            if (
                result == std::end(spheres)
                    || (*opt - origin).magnitude_sq() < (hit_point - origin).magnitude_sq()
            ) {
                hit_point = *opt;
                result = it;
            }
        }
    }

    // No sphere intersects with the ray
    if (result == std::end(spheres)) { return color::black; }

    const vec3f normal{(hit_point - result->center).normalized()};
    const auto& material{result->surface_material};
    float diffuse_intensity{0.f};
    float specular_intensity{0.f};

    // See https://en.wikipedia.org/wiki/Phong_reflection_model
    for (const auto& light : lights) {
        const vec3f light_to_hit_point{hit_point - light.src};
        const vec3f light_dir{light_to_hit_point.normalized()};

        // Check if the light is blocked by another sphere
        const std::vector<sphere>::const_iterator blocked{std::find_if(
            std::begin(spheres),
            std::end(spheres),
            [&](const sphere& s) -> bool {
                // Skip `result`, which is the sphere we are rendering, as a
                // sphere should not 'block' itself.
                if (&s == &*result) { return false; }
                if (const auto opt_vec{s.ray_hit_point(light.src, light_dir)}) {
                    // Check if `opt_vec` is in front of `result`
                    return light_to_hit_point.magnitude_sq()
                        >= (*opt_vec - light.src).magnitude_sq();
                } else {
                    return false;
                }
            }
        )};
        // This light is blocked by another sphere
        if (blocked != std::end(spheres)) { continue; }

        // Diffuse reflection
        const float cosine{-(normal * light_dir)};
        diffuse_intensity += std::max(0.f, light.intensity * cosine);

        // Specular reflection
        const vec3f reflect_dir{reflect(light_dir, normal)};
        const float reflect_align{-(reflect_dir * direction)};
        const float reflect_comp{powf(
            std::max(0.f, reflect_align),
            material.shininess
        )};
        specular_intensity += reflect_comp * light.intensity;
    }

    color reflect_color{color::black};
    color refract_color{color::black};
    if (recursion_depth > 0) {
        // Reflection
        const vec3f reflect_dir{reflect(direction, normal)};
        // Perturb `hit_point` to prevent reflecting itself.
        const vec3f reflect_hit{perturb(hit_point, reflect_dir, normal)};
        reflect_color = cast_ray(
            reflect_hit, reflect_dir, spheres, lights, recursion_depth - 1
        );

        // Refraction
        const vec3f refract_dir{
            refract(direction, normal, material.refractive_index)
        };
        // Perturb `hit_point` to prevent refracting again.
        const vec3f refract_hit{perturb(hit_point, refract_dir, normal)};
        refract_color = cast_ray(
            refract_hit, refract_dir, spheres, lights, recursion_depth - 1
        );
    }

    return diffuse_intensity * material.diffuse_color * material.diffuse_const
        + specular_intensity * color::white * material.specular_const
        + reflect_color * material.reflect_const
        + refract_color * material.refract_const;
}

std::pair<float, float> trans_scene(
    int i, int j, int width, int height, float fov_2
) {
    const float aspect_ratio = static_cast<float>(height) / width;
    const float t{tanf(fov_2)};
    return {
        ((2.f * i + 1.f) / width - 1.f) * t / aspect_ratio,
        ((2.f * j + 1.f) / height - 1.f) * t
    };
}

canvas render(
    const int width,
    const int height,
    const float fov_2,
    std::vector<sphere>& spheres,
    const std::vector<light>& lights
) {
    const vec3f origin{vec3f::zero};
    canvas cvs(width, height);

    // Sort by the distance from `origin`
    std::sort(
        std::begin(spheres),
        std::end(spheres),
        [&origin](sphere s1, sphere s2) -> bool {
            return (s1.center - origin).magnitude_sq()
                < (s2.center - origin).magnitude_sq();
        }
    );

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            const auto [x, y]{trans_scene(i, j, width, height, fov_2)};
            const auto direction{vec3f{x, y, 1}.normalized()};
            cvs[{i, j}] = cast_ray(origin, direction, spheres, lights, 4);
        }
    }
    return cvs;
}

void save_canvas(const int width, const int height, const canvas& cvs)
{
    std::vector<unsigned char> image;
    image.reserve(width * height * 4);
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            const auto rgb{cvs[{i, j}].rgb_array()};
            image.insert(std::end(image), std::begin(rgb), std::end(rgb));
            image.push_back(255);
        }
    }

    if (const auto error = lodepng::encode("out.png", image, width, height)) {
        std::cerr
            << "Encoder error " << error << ':'
            << lodepng_error_text(error) << '\n';
    }
}

int main()
try {
    lua_State *L{luaL_newstate()};
    luaL_openlibs(L);
    if (luaL_dofile(L, "env.lua") != LUA_OK) {
        lua_close(L);
        std::cerr << "env.lua not found!\n";
        return 1;
    }

    int width, height;
    float fov_2;
    lua_getglobal(L, "width");
    lua_getconst(L, width);
    lua_getglobal(L, "height");
    lua_getconst(L, height);
    lua_getglobal(L, "fov_2");
    lua_getconst(L, fov_2);

    lua_getglobal(L, "spheres");
    auto spheres{lua_getspheres(L)};

    lua_getglobal(L, "lights");
    const auto lights{lua_getlights(L)};

    lua_close(L);

    const auto cvs{render(width, height, fov_2, spheres, lights)};
    save_canvas(width, height, cvs);

    return 0;
} catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
}
