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

float& color::operator[](int i)
{
    return const_cast<float&>(const_cast<const color&>(*this)[i]);
}

const float& color::operator[](int i) const
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

const color color::red = {1.f, 0.f, 0.f};
const color color::orange = {1.f, 0.64706f, 0.f};
const color color::yellow = {1.f, 1.f, 0.f};
const color color::green = {0.f, 1.f, 0.f};
const color color::blue = {0.f, 0.f, 1.f};
const color color::indigo = {0.29412f, 0.f, 0.50980f};
const color color::purple = {0.50196f, 0.f, 0.50196f};
const color color::black = {0.f, 0.f, 0.f};
const color color::white = {1.f, 1.f, 1.f};
