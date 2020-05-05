#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"

class material {
public:
    material(const color& diffuse_color);
    color diffuse_color;
};

#endif /* ifndef MATERIAL_H */
