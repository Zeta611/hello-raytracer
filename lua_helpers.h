#ifndef LUA_HELPERS_H
#define LUA_HELPERS_H

#include <stdexcept>
#include <string>
#include <type_traits>
#include "lua.hpp"

class light;
class sphere;

class lua_exception : public std::runtime_error {
public:
    lua_exception(const std::string& func, const std::string& type);
};

template<
    typename T,
    typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
>
void lua_getconst(lua_State *L, T& target)
{
    if (!lua_isnumber(L, -1)) { throw lua_exception(__func__, "number"); }
    target = static_cast<T>(lua_tonumber(L, -1));
    lua_pop(L, 1);
}

std::vector<sphere> lua_getspheres(lua_State *L);
std::vector<light> lua_getlights(lua_State *L);

#endif /* ifndef LUA_HELPERS_H */
