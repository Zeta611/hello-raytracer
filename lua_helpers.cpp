#include "lua_helpers.h"

lua_exception::lua_exception(const std::string& func, const std::string& type)
    : std::runtime_error(
        "[" + func + "] Invalid Lua state: expected " + type
    )
{}

vec3f lua_getcenter(lua_State *L)
{
    if (!lua_istable(L, -1)) { throw lua_exception(__func__, "table"); }
    lua_getfield(L, -1, "center");
    if (!lua_istable(L, -1)) { throw lua_exception(__func__, "table"); }

    vec3f result;
    for (int i = 0; i < 3; ++i) {
        lua_pushnumber(L, i + 1);
        lua_gettable(L, -2);
        if (!lua_isnumber(L, -1)) { throw lua_exception(__func__, "number"); }
        result[i] = static_cast<float>(lua_tonumber(L, -1));
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    return result;
}

color lua_getcolor(lua_State *L)
{
    if (!lua_istable(L, -1)) { throw lua_exception(__func__, "table"); }
    lua_getfield(L, -1, "diffuse_color");
    if (!lua_istable(L, -1)) { throw lua_exception(__func__, "table"); }

    color result;
    for (int i = 0; i < 3; ++i) {
        lua_pushnumber(L, i + 1);
        lua_gettable(L, -2);
        if (!lua_isnumber(L, -1)) { throw lua_exception(__func__, "number"); }
        result[i] = static_cast<float>(lua_tonumber(L, -1));
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    return result;
}

material lua_getmaterial(lua_State *L)
{
    if (!lua_istable(L, -1)) { throw lua_exception(__func__, "table"); }
    lua_getfield(L, -1, "material");

    color diffuse_color{lua_getcolor(L)};

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

float lua_getradius(lua_State *L)
{
    if (!lua_istable(L, -1)) { throw lua_exception(__func__, "table"); }
    lua_getfield(L, -1, "radius");
    if (!lua_isnumber(L, -1)) { throw lua_exception(__func__, "number"); }

    float result = static_cast<float>(lua_tonumber(L, -1));
    lua_pop(L, 1);
    return result;
}

sphere lua_getsphere(lua_State *L)
{
    return sphere(
        lua_getcenter(L),
        lua_getradius(L),
        lua_getmaterial(L)
    );
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
