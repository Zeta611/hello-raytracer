#include <utility>
#include <vector>
#include "color.h"
#include "canvas.h"

canvas::canvas(int width, int height)
    : width(width), height(height), _canvas(width * height, color::black) {}

color& canvas::operator[](const int index)
{
    return const_cast<color&>(const_cast<const canvas&>(*this)[index]);
}

const color& canvas::operator[](const int index) const
{
    return _canvas[index];
}

color& canvas::operator[](const std::pair<int, int>& coord)
{
    return const_cast<color&>(const_cast<const canvas&>(*this)[coord]);
}

const color& canvas::operator[](const std::pair<int, int>& coord) const
{
    return _canvas[coord.first + width * coord.second];
}
