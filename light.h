#ifndef LIGHT_H
#define LIGHT_H

#include "vec.h"

class light {
public:
    light(const vec3f& position, float intensity);
    vec3f position;
    float intensity;
};

#endif /* ifndef LIGHT_H */
