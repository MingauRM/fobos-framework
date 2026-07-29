#include "lua.h"
#include "lualib.h"
#include "luaconf.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <filesystem>
#include <fstream>

static int audio_load(lua_State* L)
{
    const char* path = luaL_checkstring(L, 1);


}

static int audio_play(lua_State* L)
{

}