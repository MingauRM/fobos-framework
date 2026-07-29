#include "lua.h"
#include "lualib.h"
#include "luaconf.h"

#include <filesystem>
#include <fstream>
#include <ios>

constexpr std::string MODE_BINARY = "binary";
constexpr std::string MODE_TEXT = "text";

// Library functions

static int file_exists(lua_State* L)
{
    const char* str = luaL_checkstring(L, 1);
    std::filesystem::path filepath = std::filesystem::path(str);

    bool exists = std::filesystem::exists(filepath);
    lua_pushboolean(L, exists);

    return 1;
}

static int file_create(lua_State* L)
{
    std::filesystem::path path = luaL_checkstring(L, 1);
    std::filesystem::path parent = path.parent_path();

    if (path.has_filename())
    {
        std::error_code ec;
        std::filesystem::create_directories(parent, ec);
        if (ec)
        {
            std::string err = "Failed to create parent directories of file '" + path.generic_string() + "'. Error: " + ec.message() + ".\n";
            luaL_error(L, err.c_str());
            lua_pop(L, 1);

            return 0;
        }

        std::ofstream file(path.filename(), std::ios::out);

        if (!file.is_open())
        {
            std::string errorMessage = "Failed to open file '" + path.generic_string() + std::string("'.\n");
            luaL_error(L, errorMessage.c_str());
            lua_pop(L, 1);

            return 0;
        }

        file.close();
    }
    else
    {
        std::error_code ec;
        std::filesystem::create_directories(path, ec);
        if (ec)
        {
            std::string errorMessage = "Failed to create directory '" + path.generic_string() + "'. Error: " + ec.message() + ".\n";
            luaL_error(L, errorMessage.c_str());
            lua_pop(L, 1);

            return 0;
        }
    }

    return 0;
}

static int file_read(lua_State* L)
{
    std::filesystem::path path = luaL_checkstring(L, 1);
    std::string mode = luaL_optstring(L, 2, MODE_TEXT.c_str());

    std::ios::openmode openMode = std::ios::in;
    bool modeValid = false;

    if (mode == MODE_TEXT)
    {
        modeValid = true;
    }
    else if (mode == MODE_BINARY)
    {
        openMode |= std::ios::binary;
        modeValid = true;
    }

    if (!modeValid) 
    {
        std::string errorMessage = "Attempt to open file '" + path.generic_string() + "' with unknown mode: '" + mode + "'.\n";
        luaL_error(L, errorMessage.c_str());
        lua_pop(L, 1);

        return 0;
    }

    std::ifstream file(path, openMode);
    if (!file.is_open())
    {
        std::string errorMessage = "Failed to open file '" + path.generic_string() + "'.\n";
        luaL_error(L, errorMessage.c_str());
        lua_pop(L, 1);

        return 0;
    }

    std::string filecontent;
    file.seekg(0, std::ios::end);
    filecontent.resize(file.tellg());
    file.seekg(0, std::ios::beg);

    file.read(filecontent.data(), filecontent.size());

    lua_pushlstring(L, filecontent.data(), filecontent.size());
    return 1;
}

static int file_write(lua_State* L)
{
    std::filesystem::path path = luaL_checkstring(L, 1);
    std::string data = luaL_checkstring(L, 2);
    std::string mode = luaL_optstring(L, 3, MODE_TEXT.c_str());

    std::ios::openmode openMode = std::ios::out;
    bool modeValid = false;

    if (mode == MODE_TEXT)
    {
        modeValid = true;
    }
    else if (mode == MODE_BINARY)
    {
        openMode |= std::ios::binary;
        modeValid = true;
    }

    if (!modeValid) 
    {
        std::string errorMessage = "Attempt to open file '" + path.generic_string() + "' with unknown mode: '" + mode + "'.\n";
        luaL_error(L, errorMessage.c_str());
        lua_pop(L, 1);

        return 0;
    }

    std::ofstream file(path, std::ios::app | openMode);
    if (!file.is_open())
    {
        std::string errorMessage = "Failed to open file '" + path.generic_string() + std::string("'.\n");
        luaL_error(L, errorMessage.c_str());
        lua_pop(L, 1);

        return 0;
    }

    file << data;
    file.close();

    return 0;
}

static const luaL_Reg filelib[] = {
    {"create", file_create},
    {"exists", file_exists},
    {"read", file_read},
    {"write", file_write},
    {NULL, NULL}
};

int luaopen_file(lua_State* L)
{
    luaL_register(L, LUA_FILELIBNAME, filelib);

    lua_newtable(L);

    lua_pushstring(L, MODE_BINARY.c_str());
    lua_setfield(L, -2, "binary");

    lua_pushstring(L, MODE_TEXT.c_str());
    lua_setfield(L, -2, "text");

    lua_setfield(L, -2, "filemode");

    return 1;
}