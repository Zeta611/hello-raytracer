#ifndef SPHERE_H
#define SPHERE_H

#include <optional>
#include "material.h"
#include "vec.h"

class sphere {
public:
    sphere(
        const vec3f &center,
        const float radius,
        const material &surface_material
    );

    vec3f center;
    float radius;
    material surface_material;

    // Returns the hit point position wrapped inside an optional if the ray
    // intersects with the sphere.  Returns a nullopt otherwise.
    std::optional<vec3f> ray_hit_point(
        const vec3f &origin,
        const vec3f &direction
    ) const;
};

#endif /* ifndef SPHERE_H */
