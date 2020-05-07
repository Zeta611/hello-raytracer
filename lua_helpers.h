#ifndef LUA_HELPERS_H
#define LUA_HELPERS_H

#include <stdexcept>
#include <string>
#include <vector>
#include "color.h"
#include "lua.hpp"
#include "sphere.h"
#include "vec.h"

class lua_exception : public std::runtime_error {
public:
    lua_exception(const std::string& func, const std::string& type);
};

vec3f lua_getcenter(lua_State *L);
color lua_getcolor(lua_State *L);
material lua_getmaterial(lua_State *L);
float lua_getradius(lua_State *L);
sphere lua_getsphere(lua_State *L);
std::vector<sphere> lua_getspheres(lua_State *L);

#endif /* ifndef LUA_HELPERS_H */
