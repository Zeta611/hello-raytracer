#include <array>
#include <cmath>
#include <fstream>
#include <memory>
#include <utility>
#include "color.h"
#include "sphere.h"
#include "vec.h"

color cast_ray(
    const vec3f &origin,
    const vec3f &direction,
    const sphere& sphere
) {
    if (sphere.ray_intersects(origin, direction)) {
        return color::foreground;
    } else {
        return color::background;
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

void render()
{
    constexpr int width = 4096;
    constexpr int height = 2160;
    constexpr float fov_2 = M_PI / 3;

    const auto scene = std::make_unique<std::array<color, width * height>>();
    const sphere sphere({0.f, 0.f, 2.f}, 1.f);

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            const auto [x, y] = trans_scene(i, j, width, height, fov_2);
            (*scene)[i + j * width] = cast_ray(vec3f::zero, {x, y, 1}, sphere);
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
    render();
    return 0;
}
