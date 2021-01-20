#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"

class material {
public:
    material(
        color diffuse_color,
        float diffuse_const,
        float specular_const,
        float shininess,
        float reflect_const,
        float refract_const,
        float refractive_index
    );

    color diffuse_color;
    float diffuse_const;
    float specular_const;
    float shininess;
    float reflect_const;
    float refract_const;
    float refractive_index;

    static const material gold;
    static const material silver;
    static const material ivory;
    static const material glass;
    static const material mirror;

    static material metallize(color);
    static material rubberize(color);
};

#endif /* ifndef MATERIAL_H */
