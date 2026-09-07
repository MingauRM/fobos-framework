#pragma once

#include "engine.hpp"

constexpr int FilePathTag = 3;

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
    std::filesystem::path* udata = (std::filesystem::path*)lua_newuserdatatagged(L, sizeof(std::filesystem::path), FilePathTag);
    new (udata) std::filesystem::path(path);
    return udata;
}

static std::filesystem::path* lua_checkpath(lua_State* L, int index)
{
    std::filesystem::path* path = (std::filesystem::path*)lua_touserdatatagged(L, index, FilePathTag);
    if (!path)
        luaL_typeerrorL(L, index, "Path");
    return path;
}