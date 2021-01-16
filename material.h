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
        float reflect_const
    );

    color diffuse_color;
    float diffuse_const;
    float specular_const;
    float shininess;
    float reflect_const;

    static const material gold;
    static const material silver;
    static const material ivory;

    static material metallize(color);
    static material rubberize(color);
};

#endif /* ifndef MATERIAL_H */
