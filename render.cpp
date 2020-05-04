#include <fstream>
#include <array>
#include <iostream>
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

void render()
{
    constexpr int unit = 1000;
    constexpr int width = 4096;
    constexpr int height = 2160;
    constexpr int depth = unit;

    auto *const canvas = new std::array<color, width * height>;
    const sphere sphere({0.f, 0.f, 2.f * depth}, unit / 2.f);
    const vec3f origin{0.f, 0.f, 0.f};

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            const float x = i - width / 2;
            const float y = j - height / 2;
            const vec3f canvas_pt{x, y, depth};
            (*canvas)[i + j * width] = cast_ray(origin, canvas_pt, sphere);
        }
    }

    std::ofstream ofs;
    ofs.open("./out.ppm");
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (const auto& point : *canvas) {
        for (int i = 0; i < 3; ++i) {
            ofs << static_cast<char>(255 * point[i]);
        }
    }
    ofs.close();

    delete canvas;
}

int main()
{
    render();
    return 0;
}
