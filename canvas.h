#ifndef CANVAS_H
#define CANVAS_H

#include <utility>
#include <vector>
#include "color.h"

class canvas {
public:
    canvas(int width, int height);

    const int width;
    const int height;

    color& operator[](int);
    const color& operator[](int) const;

    color& operator[](std::pair<int, int>);
    const color& operator[](std::pair<int, int>) const;

private:
    std::vector<color> _canvas;
};

#endif /* ifndef CANVAS_H */
