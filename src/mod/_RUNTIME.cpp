#include "lua.h"
#include "lualib.h"
#include "luacode.h"

int luaopen_RUNTIME(lua_State* L)
{
    lua_newtable(L);
    int runtime_table = lua_gettop(L);

    lua_pushstring(L, "fobos");
    lua_setfield(L, runtime_table, "name");

    lua_newtable(L); // version
    int runtime_version = lua_gettop(L);

    lua_pushstring(L, "1.0.0");
    lua_setfield(L, runtime_version, "display");

    lua_setreadonly(L, -1, 1);
    lua_setfield(L, runtime_table, "version");

    lua_setreadonly(L, runtime_table, 1);
    lua_setglobal(L, LUA_RUNTIMEDATANAME);

    return 0;
}