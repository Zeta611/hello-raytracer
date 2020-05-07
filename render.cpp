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

color cast_ray(
    const vec3f &origin,
    const vec3f &direction,
    const std::vector<sphere>& spheres,
    const std::vector<light>& lights
) {
    vec3f hit_point;
    // find the first sphere that the ray intersects with and update
    // `hit_point`
    const auto result = std::find_if(
        spheres.begin(),
        spheres.end(),
        [&](const sphere& s) -> bool {
            if (const auto opt = s.ray_hit_point(origin, direction)) {
                hit_point = *opt;
                return true;
            } else {
                return false;
            }
        }
    );
    if (result == spheres.end()) { return color::black; }

    const vec3f normal = (hit_point - result->center).normalized();
    const auto& material = result->surface_material;
    float diffuse_intensity = 0.f;
    float specular_intensity = 0.f;

    // See https://en.wikipedia.org/wiki/Phong_reflection_model
    for (const auto& light : lights) {
        const vec3f light_to_hit_point = hit_point - light.src;
        const vec3f light_dir = light_to_hit_point.normalized();

        // check if the light is blocked by another sphere
        const auto blocked = std::find_if(
            spheres.begin(),
            spheres.end(),
            [&](const sphere& s) -> bool {
                // skip `result`, which is the sphere we are rendering
                if (&s == &*result) { return false; }
                if (const auto opt = s.ray_hit_point(light.src, light_dir)) {
                    // check if `opt` is in front of `result`
                    return light_to_hit_point.magnitude_sq()
                        >= (*opt - light.src).magnitude_sq();
                } else {
                    return false;
                }
            }
        );
        if (blocked != spheres.end()) { continue; }

        const float cosine = -(normal * light_dir);
        diffuse_intensity += std::max(0.f, light.intensity * cosine);

        const vec3f reflect_dir = light_dir + 2.f * cosine * normal;
        const float reflect_align = -(reflect_dir * direction.normalized());
        const float reflect_comp = std::powf(
            std::max(0.f, reflect_align),
            material.shininess
        );
        specular_intensity += reflect_comp * light.intensity;
    }
    return diffuse_intensity * material.diffuse_color * material.diffuse_const
        + specular_intensity * color::white * material.specular_const;
}

std::pair<float, float> trans_scene(
    int i, int j, float width, float height, float fov_2
) {
    const float aspect_ratio = height / width;
    const float t = std::tanf(fov_2);
    return {
        ((2.f * i + 1.f) / width - 1.f) * t / aspect_ratio,
        ((2.f * j + 1.f) / height - 1.f) * t
    };
}

void render(
    const int width,
    const int height,
    const float fov_2,
    std::vector<sphere>& spheres,
    const std::vector<light>& lights
) {
    const vec3f origin = vec3f::zero;
    const auto scene = std::make_unique<color[]>(width * height);

    // sort by the distance from `origin`
    std::sort(
        spheres.begin(),
        spheres.end(),
        [&](sphere s1, sphere s2) -> bool {
            return (s1.center - origin).magnitude_sq()
                < (s2.center - origin).magnitude_sq();
        }
    );

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            const auto [x, y] = trans_scene(i, j, width, height, fov_2);
            scene[i + j * width] = cast_ray(
                origin, {x, y, 1}, spheres, lights
            );
        }
    }

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
    constexpr int width = 2048;
    constexpr int height = 1536;
    constexpr float fov_2 = M_PI_4;

    lua_State *L = luaL_newstate();
    if (luaL_dofile(L, "env.lua") != LUA_OK) {
        lua_close(L);
        std::cerr << "env.lua not found!\n";
        return 1;
    }

    lua_getglobal(L, "spheres");
    auto spheres = lua_getspheres(L);
    lua_close(L);

    const std::vector lights = {
        light({-20.f, -20.f, -20.f}, 1.2f),
        light({30.f, -50.f, 25.f}, 1.4f),
        light({30.f, -20.f, -30.f}, .8f)
    };

    render(width, height, fov_2, spheres, lights);
    return 0;
} catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
}
