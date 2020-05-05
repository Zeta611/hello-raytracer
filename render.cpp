#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <memory>
#include <utility>
#include <vector>
#include "color.h"
#include "sphere.h"
#include "vec.h"

color cast_ray(
    const vec3f &origin,
    const vec3f &direction,
    const std::vector<sphere>& spheres
) {
    // find the first sphere that the ray intersects with
    const auto result = std::find_if(
        spheres.begin(),
        spheres.end(),
        [&](sphere s) -> bool { return s.ray_intersects(origin, direction); }
    );

    if (result != spheres.end()) {
        return result->surface_material.diffuse_color;
    } else {
        return color::black;
    }
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

void render(std::vector<sphere>& spheres)
{
    constexpr int width = 1024;
    constexpr int height = 768;
    constexpr float fov_2 = M_PI_4;
    const vec3f origin = vec3f::zero;
    const auto scene = std::make_unique<std::array<color, width * height>>();

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
            (*scene)[i + j * width] = cast_ray(origin, {x, y, 1}, spheres);
        }
    }

    std::ofstream ofs;
    ofs.open("./out.ppm");
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (const auto& point : *scene) {
        for (int i = 0; i < 3; ++i) {
            ofs << static_cast<char>(255 * point[i]);
        }
    }
    ofs.close();
}

int main()
{
    std::vector spheres = {
        sphere({-2.f, 0.f, 16.f}, 2.f, {color::red}),
        sphere({-1.f, -1.5f, 12.f}, 2.f, {color::orange}),
        sphere({1.5f, -0.5f, 18.f}, 3.f, {color::yellow}),
        sphere({7.f, 5.f, 18.f}, 4.f, {color::green}),
    };
    render(spheres);
    return 0;
}
