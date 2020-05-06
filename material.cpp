#include "material.h"

material::material(
    color diffuse_color,
    float diffuse_const,
    float specular_const,
    float shininess
) : diffuse_color(diffuse_color),
    diffuse_const(diffuse_const),
    specular_const(specular_const),
    shininess(shininess)
{}

const material material::gold(color::gold, .6f, .3f, 30.f);
const material material::silver(color::silver, .6f, .3f, 30.f);
const material material::ivory(color::ivory * .6f, .9f, .1f, 10.f);

material material::metallize(color c)
{
    return material(c, .6f, .3f, 30.f);
}

material material::rubberize(color c)
{
    return material(c, .9f, .1f, 10.f);
}
