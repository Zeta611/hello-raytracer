#ifndef SPHERE_H
#define SPHERE_H

#include "vec.h"

struct sphere {
    vec3f center;
    float radius;

    sphere(const vec3f &center, const float radius);

    bool ray_intersects(const vec3f &origin, const vec3f &direction) const;
};

#endif /* ifndef SPHERE_H */
