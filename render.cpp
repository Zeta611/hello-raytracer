#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
#include "color.h"
#include "light.h"
#include "lua.hpp"
#include "lua_helpers.h"
#include "sphere.h"
#include "vec.h"

vec3f reflect(const vec3f &direction, const vec3f &normal)
{
    // Both `direction` and `normal` should be normalized.
    const float cosine{-(normal * direction)};
    return direction + 2.f * cosine * normal;
}

vec3f perturb(const vec3f &point, const vec3f &direction, const vec3f &normal)
{
    const float k{same_direction(direction, normal) ? 1e-3f : -1e-3f};
    return k * normal + point;
}

color cast_ray(
    const vec3f &origin,
    const vec3f &direction,
    const std::vector<sphere>& spheres,
    const std::vector<light>& lights,
    const int recursion_depth = 0
) {
    vec3f hit_point;
    // Find the first sphere that the ray intersects with and update
    // `hit_point`
    const std::vector<sphere>::const_iterator result{std::find_if(
        spheres.begin(),
        spheres.end(),
        [&](const sphere& s) -> bool {
            if (const auto opt{s.ray_hit_point(origin, direction)}) {
                hit_point = *opt;
                return true;
            } else {
                return false;
            }
        }
    )};
    // No sphere intersects with the ray
    if (result == spheres.end()) { return color::black; }

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
            spheres.begin(),
            spheres.end(),
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
        if (blocked != spheres.end()) { continue; }

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
    if (recursion_depth > 0) {
        // Reflection
        const vec3f reflect_dir{reflect(direction, normal)};
        // Perturb `hit_point` to prevent reflecting itself.
        const vec3f target_point{perturb(hit_point, reflect_dir, normal)};
        reflect_color = cast_ray(
            target_point, reflect_dir, spheres, lights, recursion_depth - 1
        );
    }

    return diffuse_intensity * material.diffuse_color * material.diffuse_const
        + specular_intensity * color::white * material.specular_const
        + reflect_color * material.reflect_const;
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

std::vector<color> render(
    const int width,
    const int height,
    const float fov_2,
    std::vector<sphere>& spheres,
    const std::vector<light>& lights
) {
    const vec3f origin{vec3f::zero};

    std::vector<color> scene;
    scene.reserve(width * height);

    // Sort by the distance from `origin`
    std::sort(
        spheres.begin(),
        spheres.end(),
        [&origin](sphere s1, sphere s2) -> bool {
            return (s1.center - origin).magnitude_sq()
                < (s2.center - origin).magnitude_sq();
        }
    );

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            const auto [x, y]{trans_scene(i, j, width, height, fov_2)};
            const auto direction{vec3f{x, y, 1}.normalized()};
            scene.push_back(cast_ray(origin, direction, spheres, lights, 8));
        }
    }
    return scene;
}

void save_scene(const int width, const int height, const std::vector<color>& scene)
{
    std::ofstream ofs;
    ofs.open("./out.ppm");
    ofs << "P6\n" << width << " " << height << "\n255\n";

    for (int i = 0; i < width * height; ++i) {
        for (int j = 0; j < 3; ++j) {
            ofs << static_cast<char>(255 * scene[i][j]);
        }
    }
    ofs.close();
}

int main()
try {
    constexpr int width{2048};
    constexpr int height{1536};
    constexpr float fov_2{M_PI_4};

    lua_State *L{luaL_newstate()};
    if (luaL_dofile(L, "env.lua") != LUA_OK) {
        lua_close(L);
        std::cerr << "env.lua not found!\n";
        return 1;
    }

    lua_getglobal(L, "spheres");
    auto spheres{lua_getspheres(L)};
    lua_close(L);

    const std::vector lights{
        light({-20.f, -20.f, -20.f}, 1.2f),
        light({30.f, -50.f, 25.f}, 1.4f),
        light({30.f, -20.f, -30.f}, .8f)
    };

    const auto scene{render(width, height, fov_2, spheres, lights)};
    save_scene(width, height, scene);

    return 0;
} catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
}
