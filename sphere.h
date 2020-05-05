#ifndef SPHERE_H
#define SPHERE_H

#include "material.h"
#include "vec.h"

class sphere {
public:
    vec3f center;
    float radius;
    material surface_material;

    sphere(
        const vec3f &center,
        const float radius,
        const material &surface_material
    );

    bool ray_intersects(const vec3f &origin, const vec3f &direction) const;
};

#endif /* ifndef SPHERE_H */
