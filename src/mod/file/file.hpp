#pragma once

#include "engine.hpp"

bool is_path_safe(std::filesystem::path path)
{
    std::error_code ec;

    std::filesystem::path canonicalRoot = std::filesystem::weakly_canonical(GAME_ROOT, ec);
    if (ec) return false;

    std::filesystem::path sanitizedPath = std::filesystem::weakly_canonical(path, ec);
    if (ec) return false;

    std::filesystem::path relativePath = std::filesystem::relative(sanitizedPath, canonicalRoot, ec);
    if (ec || relativePath.empty())
    {
        return false;
    }
    
    auto firstComponent = relativePath.begin();
    if (firstComponent != relativePath.end() && *firstComponent == "..")
    {
        return false;
    }

    return true;
}

static std::filesystem::path* lua_pushpath(lua_State* L, std::filesystem::path path)
{
    std::filesystem::path* udata = (std::filesystem::path*)lua_newuserdatataggedwithmetatable(L, sizeof(std::filesystem::path), (int)CustomLuauTypeTags::FILEPATH_TYPE);
    new (udata) std::filesystem::path(path);
    return udata;
}

static std::filesystem::path* lua_checkpath(lua_State* L, int index)
{
    std::filesystem::path* path = (std::filesystem::path*)lua_touserdatatagged(L, index, (int)CustomLuauTypeTags::FILEPATH_TYPE);
    if (!path)
    {
        luaL_typeerrorL(L, index, FILEPATH_TYPENAME);
    }
    
    return path;
}

static int path_add(lua_State* L)
{
    std::filesystem::path a = *lua_checkpath(L, 1);
    std::filesystem::path b = *lua_checkpath(L, 2);

    std::filesystem::path finalPath = a /= b;

    lua_pushpath(L, finalPath);

    return 1;
}

int path_tostring(lua_State* L)
{
    std::filesystem::path path = *lua_checkpath(L, 1);
    const char* str = path.generic_string().c_str();

    lua_pushlstring(L, str, sizeof(str));
    return 1;
}

int path_index(lua_State* L)
{
    std::filesystem::path path = *lua_checkpath(L, 1);
    const char* key = luaL_checkstring(L, 2);

    if (strcmp(key, "Parent") == 0)
    {
        lua_pushpath(L, path.parent_path());
        return 1;
    }

    if (strcmp(key, "Extension") == 0)
    {
        lua_pushpath(L, path.extension());
        return 1;
    }

    if (strcmp(key, "Name") == 0)
    {
        lua_pushpath(L, path.filename());
        return 1;
    }

    luaL_error(L, "'%s' is not a valid member of FilePath", key);
    return 0;
}