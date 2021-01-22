#ifndef CANVAS_H
#define CANVAS_H

#include <utility>
#include <vector>

class color;

class canvas {
public:
    canvas(int width, int height);

    const int width;
    const int height;

    color& operator[](const int);
    const color& operator[](const int) const;

    color& operator[](const std::pair<int, int>&);
    const color& operator[](const std::pair<int, int>&) const;

private:
    std::vector<color> _canvas;
};

#endif /* ifndef CANVAS_H */
