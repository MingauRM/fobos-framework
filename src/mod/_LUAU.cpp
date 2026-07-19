#include "lua.h"
#include "lualib.h"
#include "luacode.h"

int luaopen_LUAU(lua_State* L)
{
    lua_newtable(L);
    int luau_table = lua_gettop(L);

    lua_pushstring(L, "https://github.com/mingaurm/fobos-framework");
    lua_setfield(L, luau_table, "url");

    lua_newtable(L); // version
    int luau_version = lua_gettop(L);

    lua_pushstring(L, "0.729");
    lua_setfield(L, luau_version, "display");

    lua_pushnumber(L, 0);
    lua_setfield(L, luau_version, "major");

    lua_pushnumber(L, 729);
    lua_setfield(L, luau_version, "minor");

    lua_setreadonly(L, luau_version, 1);
    lua_setfield(L, luau_table, "version");

    lua_setreadonly(L, luau_table, 1);
    lua_setglobal(L, LUAU_VERSIONNAME);

    return 0;
}