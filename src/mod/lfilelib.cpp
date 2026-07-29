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
    std::filesystem::path filepath = luaL_checkstring(L, 1);

    bool exists = std::filesystem::exists(filepath);
    lua_pushboolean(L, exists);

    return 1;
}

static int file_create(lua_State* L)
{
    std::filesystem::path path = luaL_checkstring(L, 1);
    
    if (path.has_filename())
    {
        std::filesystem::path parent = path.parent_path();

        std::error_code ec;
        std::filesystem::create_directories(parent, ec);
        if (ec)
        {
            std::string err = "Failed to create parent directories of file '" + path.generic_string() + "'. Error: " + ec.message() + ".\n";
            luaL_error(L, err.c_str());
            lua_pop(L, 1);

            return 0;
        }

        std::ofstream file(path, std::ios::out);

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
    if (!std::filesystem::exists(path))
    {
        std::string errorMessage = "File at '" + path.generic_string() + "' does not exist.\n";
        luaL_error(L, errorMessage.c_str());
        lua_pop(L, 1);

        return 0;
    }

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
    if (!std::filesystem::exists(path))
    {
        std::string errorMessage = "File at '" + path.generic_string() + "' does not exist.\n";
        luaL_error(L, errorMessage.c_str());
        lua_pop(L, 1);

        return 0;
    }

    std::string data = luaL_checkstring(L, 2);
    int position = luaL_checkinteger(L, 3);
    std::string mode = luaL_optstring(L, 4, MODE_TEXT.c_str());

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

    std::ofstream file(path, std::ios::ate | openMode);
    if (!file.is_open())
    {
        std::string errorMessage = "Failed to open file '" + path.generic_string() + std::string("'.\n");
        luaL_error(L, errorMessage.c_str());
        lua_pop(L, 1);

        return 0;
    }

    file.seekp(position, std::ios::beg);

    file.write(data.data(), data.size());
    file.close();

    return 0;
}

static int file_isfolder(lua_State* L)
{
    std::filesystem::path dir = luaL_checkstring(L, 1);
    lua_pushboolean(L, std::filesystem::is_directory(dir));

    return 1;
}

static int file_isfile(lua_State* L)
{
    std::filesystem::path file = luaL_checkstring(L, 1);
    lua_pushboolean(L, std::filesystem::is_regular_file(file));

    return 1;
}

static int file_getchildren(lua_State* L)
{
    std::filesystem::path path = luaL_checkstring(L, 1);
    if (!std::filesystem::exists(path))
    {
        std::string errorMessage = "Folder at '" + path.generic_string() + "' does not exist.\n";
        luaL_error(L, errorMessage.c_str());
        lua_pop(L, 1);

        return 0;
    }
    
    if (!std::filesystem::is_directory(path))
    {
        std::string errorMessage = "Expected path to result in a directory, but got " + std::string(std::filesystem::is_regular_file(path) ? "file" : "unknown");
        luaL_error(L, errorMessage.c_str());
        lua_pop(L, 1);

        return 0;
    }

    lua_newtable(L);
    size_t key = 0;

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        const char* entryPath = entry.path().generic_string().c_str();
        lua_pushstring(L, entryPath);
        lua_rawseti(L, -2, key++);
    }

    return 1;
}

static int file_getdescendants(lua_State* L)
{
    std::filesystem::path path = luaL_checkstring(L, 1);
    if (!std::filesystem::exists(path))
    {
        std::string errorMessage = "Folder at '" + path.generic_string() + "' does not exist.\n";
        luaL_error(L, errorMessage.c_str());
        lua_pop(L, 1);

        return 0;
    }
    
    if (!std::filesystem::is_directory(path))
    {
        std::string errorMessage = "Expected path to result in a directory, but got " + std::string(std::filesystem::is_regular_file(path) ? "file" : "unknown");
        luaL_error(L, errorMessage.c_str());
        lua_pop(L, 1);

        return 0;
    }

    lua_newtable(L);
    size_t key = 0;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
    {
        const char* entryPath = entry.path().generic_string().c_str();
        lua_pushstring(L, entryPath);
        lua_rawseti(L, -2, key++);
    }

    return 1;
}

static const luaL_Reg filelib[] = {
    {"create", file_create},
    {"exists", file_exists},
    {"read", file_read},
    {"write", file_write},
    {"isfile", file_isfile},
    {"isfolder", file_isfolder},
    {"getchildren", file_getchildren},
    {"getdescendants", file_getdescendants},
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