#include <stdexcept>
#include <string>
#include <utility>
#include "color.h"

color::color() {}

color::color(std::initializer_list<float> lst)
{
    for (
        auto [it, i] = std::pair{lst.begin(), 0};
        i < 3 && it != lst.end();
        ++i, ++it
    ) {
        switch (i) {
        case 0:
            red = *it;
            break;
        case 1:
            green = *it;
            break;
        case 2:
            blue = *it;
            break;
        default:
            break;
        }
    }
}

float& color::operator[](int i)
{
    return const_cast<float&>(const_cast<const color&>(*this)[i]);
}

const float& color::operator[](int i) const
{
    switch (i) {
    case 0:
        return red;
    case 1:
        return green;
    case 2:
        return blue;
    default:
        throw std::runtime_error("Index out of range");
    }
}

const color color::background = {0.f, 0.f, 0.f};
const color color::foreground = {1.f, 1.f, 1.f};
