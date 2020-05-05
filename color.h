#ifndef COLOR_H
#define COLOR_H

#include <cassert>
#include <initializer_list>

class color {
public:
    color();
    color(std::initializer_list<float> lst);

    float red_comp = 0.f;
    float green_comp = 0.f;
    float blue_comp = 0.f;

    float& operator[](int i);
    const float& operator[](int i) const;

    static const color red;
    static const color orange;
    static const color yellow;
    static const color green;
    static const color blue;
    static const color indigo;
    static const color purple;
    static const color black;
    static const color white;
};

#endif /* ifndef COLOR_H */
