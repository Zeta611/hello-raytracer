#include <utility>
#include <vector>
#include "color.h"
#include "canvas.h"

canvas::canvas(int width, int height)
    : width(width), height(height), _canvas(width * height, color::black) {}

color& canvas::operator[](int index)
{
    return const_cast<color&>(const_cast<const canvas&>(*this)[index]);
}

const color& canvas::operator[](int index) const
{
    return _canvas[index];
}

color& canvas::operator[](std::pair<int, int> coord)
{
    return const_cast<color&>(const_cast<const canvas&>(*this)[coord]);
}

const color& canvas::operator[](std::pair<int, int> coord) const
{
    return _canvas[coord.first + width * coord.second];
}
