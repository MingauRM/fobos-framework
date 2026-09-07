#include "engine.hpp"
#include "file/file.hpp"

int lua_require(lua_State* L)
{
    const char* path = luaL_checkstring(L, 1);
    if (!is_path_safe(path))
    {
        luaL_error(L, ("Attempt to require script on inaccessible directory. (Must be descendant of " + GAME_ROOT.generic_string() + ").\n").c_str());
        lua_pop(L, 1);
        return 0;
    }

    
}