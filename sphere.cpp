#include <cmath>
#include <optional>
#include "sphere.h"
#include "util.h"

sphere::sphere(
    const vec3f& center,
    const float radius,
    const material& surface_material
) : center(center), radius(radius), surface_material(surface_material) {}

std::optional<vec3f> sphere::ray_hit_point(
    const vec3f& origin,
    const vec3f& direction
) const {
    // Solve:
    // ||x - center|| = radius
    // x = origin + t direction (0 <= t, ||direction|| = 1)
    const vec3f origin_center{center - origin};
    const float inner_prod{direction * origin_center};
    const float discriminant{
        square(radius) - origin_center.magnitude_sq() + square(inner_prod)
    };
    if (discriminant < 0.f) { return {}; }

    const float sqrt_d{sqrtf(discriminant)};
    if (const float t1{inner_prod - sqrt_d}; t1 >= 0.f) {
        return origin + t1 * direction;
    } else if (const float t2{inner_prod + sqrt_d}; t2 >= 0.f) {
        return origin + t2 * direction;
    } else {
        return {};
    }
}
