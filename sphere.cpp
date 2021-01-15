#include <cmath>
#include <optional>
#include "sphere.h"

sphere::sphere(
    const vec3f &center,
    const float radius,
    const material &surface_material
) : center(center), radius(radius), surface_material(surface_material) {}

std::optional<vec3f> sphere::ray_hit_point(
    const vec3f &origin,
    const vec3f &direction
) const {
    const vec3f center_origin{center - origin};
    const vec3f norm_direction{direction.normalized()};
    const float origin_to_center_sq{center_origin.magnitude_sq()};
    const float origin_to_perpend{center_origin * norm_direction};
    const float radius_sq{radius * radius};

    if (origin_to_perpend < 0.f && origin_to_center_sq > radius_sq) {
        // sphere is behind the ray and does not intersect
        return {};
    }

    const float center_to_perpend_sq{origin_to_center_sq
        - origin_to_perpend * origin_to_perpend};
    if (center_to_perpend_sq > radius_sq) {
        // sphere is in front of the ray and does not intersect
        return {};
    }

    // sphere intersects with the ray
    const float hit_point_to_perpend_sq{radius_sq - center_to_perpend_sq};
    const float origin_to_hit_point{(origin_to_perpend >= 0.f) *
        (origin_to_perpend - sqrtf(hit_point_to_perpend_sq))};
    return origin + origin_to_hit_point * norm_direction;
}
