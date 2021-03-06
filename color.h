#ifndef COLOR_H
#define COLOR_H

#include <array>
#include <initializer_list>

class color {
public:
    color();
    color(float red, float green, float blue);
    color(std::initializer_list<float> lst);

    float red_comp = 0.f;
    float green_comp = 0.f;
    float blue_comp = 0.f;

    float& operator[](const int i);
    const float& operator[](const int i) const;

    std::array<unsigned char, 3> rgb_array() const;

    static const color red;
    static const color orange;
    static const color yellow;
    static const color green;
    static const color blue;
    static const color indigo;
    static const color purple;
    static const color black;
    static const color white;

    static const color gold;
    static const color silver;
    static const color ivory;
};

color operator+(const color&, const color&);
color operator*(float, const color&);
color operator*(const color&, float);

#endif /* ifndef COLOR_H */
