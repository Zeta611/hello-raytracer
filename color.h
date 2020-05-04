#ifndef COLOR_H
#define COLOR_H

#include <cassert>
#include <initializer_list>

struct color {
    color();
    color(std::initializer_list<float> lst);

    float red = 0.f;
    float green = 0.f;
    float blue = 0.f;

    float& operator[](int i);
    const float& operator[](int i) const;

    static const color background;
    static const color foreground;
};

#endif /* ifndef COLOR_H */

