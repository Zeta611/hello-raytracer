#include <algorithm>
#include <array>
#include <stdexcept>
#include <utility>
#include "color.h"

color::color() {}

color::color(float red, float green, float blue)
    : red_comp(red), green_comp(green), blue_comp(blue) {}

color::color(std::initializer_list<float> lst)
{
    for (
        auto [it, i]{std::pair{lst.begin(), 0}};
        i < 3 && it != lst.end();
        ++i, ++it
    ) {
        switch (i) {
        case 0:
            red_comp = *it;
            break;
        case 1:
            green_comp = *it;
            break;
        case 2:
            blue_comp = *it;
            break;
        default:
            break;
        }
    }
}

float& color::operator[](const int i)
{
    return const_cast<float&>(const_cast<const color&>(*this)[i]);
}

const float& color::operator[](const int i) const
{
    switch (i) {
    case 0:
        return red_comp;
    case 1:
        return green_comp;
    case 2:
        return blue_comp;
    default:
        throw std::runtime_error("Index out of range");
    }
}

std::array<unsigned char, 3> color::rgb_array() const
{
    return {
        static_cast<unsigned char>(red_comp * 255),
        static_cast<unsigned char>(green_comp * 255),
        static_cast<unsigned char>(blue_comp * 255)
    };
}

const color color::red{1.f, 0.f, 0.f};
const color color::orange{1.f, 0.64706f, 0.f};
const color color::yellow{1.f, 1.f, 0.f};
const color color::green{0.f, 1.f, 0.f};
const color color::blue{0.f, 0.f, 1.f};
const color color::indigo{0.29412f, 0.f, 0.50980f};
const color color::purple{0.50196f, 0.f, 0.50196f};
const color color::black{0.f, 0.f, 0.f};
const color color::white{1.f, 1.f, 1.f};

const color color::gold{1.f, 0.74314f, 0.f};
const color color::silver{0.66667f, 0.66275f, 0.67843f};
const color color::ivory{1.f, 1.f, 0.62745f};

color operator+(const color& lhs, const color& rhs)
{
    return color(
        std::min(1.f, lhs.red_comp + rhs.red_comp),
        std::min(1.f, lhs.green_comp + rhs.green_comp),
        std::min(1.f, lhs.blue_comp + rhs.blue_comp)
    );
}

color operator*(float k, const color& c)
{
    return color(
        std::min(1.f, std::max(0.f, k * c.red_comp)),
        std::min(1.f, std::max(0.f, k * c.green_comp)),
        std::min(1.f, std::max(0.f, k * c.blue_comp))
    );
}

color operator*(const color& c, float k)
{
    return color(
        std::min(1.f, std::max(0.f, k * c.red_comp)),
        std::min(1.f, std::max(0.f, k * c.green_comp)),
        std::min(1.f, std::max(0.f, k * c.blue_comp))
    );
}
