#include <stdexcept>
#include <string>
#include <vector>
#include "color.h"
#include "light.h"
#include "lua.hpp"
#include "lua_helpers.h"
#include "sphere.h"
#include "vec.h"

lua_exception::lua_exception(const std::string& func, const std::string& type)
    : std::runtime_error(
        "[" + func + "] Invalid Lua state: expected " + type
    )
{}

void get_field(lua_State *L, const char *field, float& target)
{
    if (!lua_istable(L, -1)) { throw lua_exception(__func__, "table"); }
    lua_getfield(L, -1, field);
    if (!lua_isnumber(L, -1)) { throw lua_exception(__func__, "number"); }

    target = static_cast<float>(lua_tonumber(L, -1));
    lua_pop(L, 1);
}

template<typename T>
void get_field(lua_State *L, const char *field, T& target)
{
    if (!lua_istable(L, -1)) { throw lua_exception(__func__, "table"); }
    lua_getfield(L, -1, field);
    if (!lua_istable(L, -1)) { throw lua_exception(__func__, "table"); }

    for (int i = 0; i < 3; ++i) {
        lua_pushnumber(L, i + 1);
        lua_gettable(L, -2);
        if (!lua_isnumber(L, -1)) { throw lua_exception(__func__, "number"); }
        target[i] = static_cast<float>(lua_tonumber(L, -1));
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
}

material lua_getmaterial(lua_State *L)
{
    if (!lua_istable(L, -1)) { throw lua_exception(__func__, "table"); }
    lua_getfield(L, -1, "material");

    color diffuse_color;
    get_field(L, "diffuse_color", diffuse_color);

    const char *fields[6]{
        "diffuse_const", "specular_const", "shininess", "reflect_const",
        "refract_const", "refractive_index"
    };
    float values[6];
    for (int i = 0; i < 6; ++i) {
        lua_getfield(L, -1, fields[i]);
        if (!lua_isnumber(L, -1)) { throw lua_exception(__func__, "number"); }

        values[i] = static_cast<float>(lua_tonumber(L, -1));
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    return material(
        diffuse_color,
        values[0], values[1], values[2], values[3], values[4], values[5]
    );
}

sphere lua_getsphere(lua_State *L)
{
    vec3f center;
    float radius;
    get_field(L, "center", center);
    get_field(L, "radius", radius);
    return sphere(center, radius, lua_getmaterial(L));
}

std::vector<sphere> lua_getspheres(lua_State *L)
{
    if (!lua_istable(L, -1)) { throw lua_exception(__func__, "table"); }
    std::vector<sphere> result;
    for (int i = 0; i < luaL_len(L, -1); ++i) {
        lua_pushnumber(L, i + 1);
        lua_gettable(L, -2);
        result.push_back(lua_getsphere(L));
        lua_pop(L, 1);
    }
    return result;
}

light lua_getlight(lua_State *L)
{
    vec3f src;
    float intensity;
    get_field(L, "src", src);
    get_field(L, "intensity", intensity);
    return light(src, intensity);
}

std::vector<light> lua_getlights(lua_State *L)
{
    if (!lua_istable(L, -1)) { throw lua_exception(__func__, "table"); }
    std::vector<light> result;
    for (int i = 0; i < luaL_len(L, -1); ++i) {
        lua_pushnumber(L, i + 1);
        lua_gettable(L, -2);
        result.push_back(lua_getlight(L));
        lua_pop(L, 1);
    }
    return result;
}
