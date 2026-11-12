#include <cstring>
#include "file.hpp"

constexpr const char* OPENMODE_BINARY = "binary";
constexpr const char* OPENMODE_TEXT = "text";

static int file_topath(lua_State* L)
{
    const char* pathStr = luaL_checkstring(L, 1);
    std::filesystem::path path = std::filesystem::path(pathStr);
    lua_pushpath(L, path);
    return 1;
}

static int file_tostring(lua_State* L)
{
    std::filesystem::path path = *lua_checkpath(L, 1);
    lua_pushstring(L, path.generic_string().c_str());
    return 1;
}

static int file_exists(lua_State* L)
{
    std::filesystem::path path = *lua_checkpath(L, 1);
    if (!is_path_safe(path))
    {
        luaL_error(L, ("Attempt to access file/directory without permission. (Must be descendant of " + GAME_ROOT.generic_string() + ").\n").c_str());
        lua_pop(L, 1);
        return 0;
    }

    lua_pushboolean(L, std::filesystem::exists(path));
    return 1;
}

static int file_isfile(lua_State* L)
{
    std::filesystem::path path = *lua_checkpath(L, 1);
    if (!is_path_safe(path))
    {
        luaL_error(L, ("Attempt to access file/directory without permission. (Must be descendant of " + GAME_ROOT.generic_string() + ").\n").c_str());
        lua_pop(L, 1);
        return 0;
    }

    lua_pushboolean(L, std::filesystem::is_regular_file(path));
    return 1;
}

static int file_isdir(lua_State* L)
{
    std::filesystem::path path = *lua_checkpath(L, 1);
    if (!is_path_safe(path))
    {
        luaL_error(L, ("Attempt to access file/directory without permission. (Must be descendant of " + GAME_ROOT.generic_string() + ").\n").c_str());
        lua_pop(L, 1);
        return 0;
    }

    lua_pushboolean(L, std::filesystem::is_directory(path));
    return 1;
}

static int file_getparent(lua_State* L)
{
    std::filesystem::path path = *lua_checkpath(L, 1);
    if (!is_path_safe(path))
    {
        luaL_error(L, ("Attempt to access file/directory without permission. (Must be descendant of " + GAME_ROOT.generic_string() + ").\n").c_str());
        lua_pop(L, 1);
        return 0;
    }

    lua_pushpath(L, path.parent_path());
    return 1;
}

static int file_getchildren(lua_State* L)
{
    std::filesystem::path path = *lua_checkpath(L, 1);
    if (!is_path_safe(path))
    {
        luaL_error(L, ("Attempt to access file/directory without permission. (Must be descendant of " + GAME_ROOT.generic_string() + ").\n").c_str());
        lua_pop(L, 1);
        return 0;
    }


    if (!std::filesystem::exists(path))
    {
        luaL_error(L, "Expected path to point to a directory.\n");
        lua_pop(L, 1);
        return 0;
    }

    if (!std::filesystem::is_directory(path))
    {
        luaL_error(L, "Expected path to point to a directory.\n");
        lua_pop(L, 1);
        return 0;
    }

    lua_newtable(L);
    int tbl = lua_gettop(L);

    size_t i = 1;
    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        lua_pushpath(L, entry.path());
        lua_rawseti(L, tbl, i);
        i++;
    }

    return 1;
}

static int file_read(lua_State* L)
{
    std::filesystem::path path = *lua_checkpath(L, 1);
    if (!is_path_safe(path))
    {
        luaL_error(L, ("Attempt to access file/directory without permission. (Must be descendant of " + GAME_ROOT.generic_string() + ").\n").c_str());
        lua_pop(L, 1);
        return 0;
    }

    if (!std::filesystem::is_regular_file(path))
    {
        luaL_error(L, "Expected path to point to a file.\n");
        lua_pop(L, 1);
        return 0;
    }

    const char* openModeStr = luaL_checkstring(L, 2);
    std::ios::openmode openMode;

    if (std::strcmp(openModeStr, OPENMODE_BINARY) == 0)
    {
        openMode = std::ios::binary;
    }
    else
    {
        openMode = std::ios::in;
    }

    std::ifstream file(path, openMode);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file at '" << path.generic_string() << "'.";
        lua_pushnil(L);
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    const char* contents = buffer.str().c_str();
    lua_pushstring(L, contents);

    file.close();
    return 1;
}

static int file_write(lua_State* L)
{
    std::filesystem::path path = *lua_checkpath(L, 1);
    if (!is_path_safe(path))
    {
        luaL_error(L, ("Attempt to access file/directory without permission. (Must be descendant of " + GAME_ROOT.generic_string() + ").\n").c_str());
        lua_pop(L, 1);
        return 0;
    }

    const char* content = luaL_checkstring(L, 2);
    const char* openModeStr = luaL_checkstring(L, 3);
    std::ios::openmode openMode;
    if (std::strcmp(openModeStr, OPENMODE_BINARY) == 0)
    {
        openMode = std::ios::binary;
    }
    else
    {
        openMode = std::ios::out;
    }

    std::ofstream file(path, openMode);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file at '" << path.generic_string() << "'.";
        lua_pushboolean(L, false);
        return 1;
    }

    file << content;

    file.close();

    lua_pushboolean(L, true);
    return 1;
}

static int file_makedir(lua_State* L)
{
    std::filesystem::path path = *lua_checkpath(L, 1);
    if (!is_path_safe(path))
    {
        luaL_error(L, ("Attempt to access file/directory without permission. (Must be descendant of " + GAME_ROOT.generic_string() + ").\n").c_str());
        lua_pop(L, 1);
        return 0;
    }


    std::error_code ec;
    std::filesystem::create_directory(path, ec);

    if (ec)
    {
        std::cerr << "Failed to create directory at '" << path << "' with error: '" << ec.message() << "'\n";
        
        lua_pushboolean(L, false);
        return 1;
    }

    lua_pushboolean(L, true);
    return 1;
}

static int file_remove(lua_State* L)
{
    std::filesystem::path path = *lua_checkpath(L, 1);
    if (!is_path_safe(path))
    {
        luaL_error(L, ("Attempt to access file/directory without permission. (Must be descendant of " + GAME_ROOT.generic_string() + ").\n").c_str());
        lua_pop(L, 1);
        return 0;
    }

    std::error_code ec;
    std::filesystem::remove_all(path, ec);

    if (ec)
    {
        std::cerr << "Failed to remove file '" << path << "' with error: '" << ec.message() << "'\n";
        lua_pushboolean(L, false);
        return 1;
    }

    lua_pushboolean(L, true);
    return 1;
}

static int file_copy(lua_State* L)
{
    std::filesystem::path path = *lua_checkpath(L, 1);
    std::filesystem::path destination = *lua_checkpath(L, 2);
    if (!is_path_safe(path) || !is_path_safe(destination))
    {
                luaL_error(L, ("Attempt to access file/directory without permission. (Must be descendant of " + GAME_ROOT.generic_string() + ").\n").c_str());
        lua_pop(L, 1);
        return 0;
    }

    std::error_code ec;
    std::filesystem::copy(path, destination, ec);

    if (ec)
    {
        std::cerr << "Failed to copy file from '" << path << "' to '" << destination << "' with error: '" << ec.message() << "'\n";
        lua_pushboolean(L, false);
        return 1;
    }

    lua_pushboolean(L, true);
    return 1;
}

static int file_move(lua_State* L)
{
    std::filesystem::path path = *lua_checkpath(L, 1);
    std::filesystem::path destination = *lua_checkpath(L, 2);
    if (!is_path_safe(path) || !is_path_safe(destination))
    {
        luaL_error(L, ("Attempt to access file/directory without permission. (Must be descendant of " + GAME_ROOT.generic_string() + ").\n").c_str());
        lua_pop(L, 1);
        return 0;
    }

    std::error_code ec;
    std::filesystem::rename(path, destination);

    if (ec)
    {
        std::cerr << "Failed to move file from '" << path << "' to '" << destination << "' with error: '" << ec.message() << "'\n";
        lua_pushboolean(L, false);
        return 1;
    }

    lua_pushboolean(L, true);
    return 1;
}

static int file_join(lua_State* L)
{
    size_t nArgs = lua_gettop(L);
    std::filesystem::path finalPath = *lua_checkpath(L, 1);

    for (size_t i = 2; i <= nArgs; i++)
    {
        std::filesystem::path pathArg = *lua_checkpath(L, i);
        finalPath /= pathArg;
    }

    lua_pushpath(L, finalPath);
    return 1;
}

static int file_gameroot(lua_State* L)
{
    lua_pushpath(L, GAME_ROOT);
    return 1;
}

static int file_appdata(lua_State* L)
{
    return 0;
}

// Library registry

static const luaL_Reg filelib[] = {
    {"topath", file_topath},
    {"tostring", file_tostring},
    {"exists", file_exists},
    {"isfile", file_isfile},
    {"isdir", file_isdir},
    {"getparent", file_getparent},
    {"getchildren", file_getchildren},
    {"read", file_read},
    {"write", file_write},
    {"makedir", file_makedir},
    {"remove", file_remove},
    {"copy", file_copy},
    {"move", file_move},
    {"join", file_join},
    {"gameroot", file_gameroot},
    {NULL, NULL}
};

int luaopen_file(lua_State* L)
{
    luaL_newmetatable(L, "pathMetatable");
    int metatable = lua_gettop(L);
    
    lua_pushstring(L, FILEPATH_TYPENAME);
    lua_setfield(L, metatable, "__type");

    lua_pushcfunction(L, path_index, "path_index");
    lua_setfield(L, metatable, "__index");
    lua_pushcfunction(L, path_tostring, "path_tostring");
    lua_setfield(L, metatable, "__tostring");
    lua_pushcfunction(L, path_add, "path_add");
    lua_setfield(L, metatable, "__add");

    lua_setuserdatametatable(L, (int)CustomLuauTypeTags::FILEPATH_TYPE);

    lua_newtable(L);
    int openmodeTable = lua_gettop(L);

    lua_pushlstring(L, OPENMODE_BINARY, sizeof(OPENMODE_BINARY));
    lua_setfield(L, openmodeTable, "binary");
    lua_pushlstring(L, OPENMODE_TEXT, sizeof(OPENMODE_TEXT));
    lua_setfield(L, openmodeTable, "text");

    luaL_register(L, LUA_FILELIBNAME, filelib);
    lua_setfield(L, openmodeTable, "openmode");

    return 1;
}