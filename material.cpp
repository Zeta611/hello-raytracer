#include "material.h"

material::material(
    color diffuse_color,
    float diffuse_const,
    float specular_const,
    float shininess,
    float reflect_const,
    float refract_const,
    float refractive_index
) : diffuse_color(diffuse_color),
    diffuse_const(diffuse_const),
    specular_const(specular_const),
    shininess(shininess),
    reflect_const(reflect_const),
    refract_const(refract_const),
    refractive_index(refractive_index)
{}

const material material::gold(color::gold, .6f, .3f, 30.f, .4f, 0.f, 1.f);
const material material::silver(color::silver, .6f, .3f, 30.f, .4f, 0.f, 1.f);
const material material::ivory(color::ivory * .6f, .9f, .1f, 10.f, .01f, 0.f, 1.f);
const material material::glass({.6f, .7f, .8f}, 0.f, .5f, 125.f, .1f, .8f, 1.5f);
const material material::mirror(color::white, 0.f, 10.f, 1425.f, .8f, 0.f, 1.f);

material material::metallize(color c)
{
    return material(c, .6f, .3f, 30.f, .4f, 0.f, 1.f);
}

material material::rubberize(color c)
{
    return material(c, .9f, .1f, 10.f, 0.f, 0.f, 1.f);
}
