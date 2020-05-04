#include "sphere.h"

sphere::sphere(const vec3f &center, const float radius)
    : center(center), radius(radius) {}

bool sphere::ray_intersects(const vec3f &origin, const vec3f &direction) const
{
    const vec3f center_origin = center - origin;
    const float dot_prod = center_origin * direction;
    const float mag_sq = center_origin.magnitude_sq();
    if (dot_prod < 0.f) {
        return mag_sq <= radius * radius;
    } else {
        return mag_sq - dot_prod * dot_prod / direction.magnitude_sq()
            <= radius * radius;
    }
}
