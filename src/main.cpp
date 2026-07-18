#include "lua.h"
#include "lualib.h"
#include "luaconf.h"

#include "Luau/Compiler.h"
#include "luacode.h"

#include "formatting.hpp"

#include <fstream>
#include <iostream>
#include <memory>

int execute_luau_code(lua_State* L, const std::string& chunkName, const std::string& bytecode)
{
    lua_State* T = lua_newthread(L);
    luaL_sandboxthread(T);

    int topres = lua_gettop(T);
    int result = luau_load(T, chunkName.c_str(), bytecode.data(), bytecode.size(), 0);
    if (result != 0) {
        luaL_error(L, "Failed to load bytecode for module: %s", lua_tostring(L, -1));
        lua_pop(L, 1);
        return 0;
    }  

    int status = lua_resume(T, nullptr, 0);
    if (status != LUA_OK) {
        std::cout << (status == LUA_YIELD ? "Encountered an unexpected yield during Luau execution." : "Could not execute module: " + std::string(lua_tostring(T, -1)));
        lua_pop(L, -1);
        
        return 0;
    }

    lua_pop(L, 1);

    return 1;
}

std::string compile_script(const std::string& scriptPath) {
    std::ifstream file(scriptPath, std::ios::in | std::ios::binary);
    if (!file)
    {
        std::cout << "Could not open script file: " << scriptPath << "\n";
        return "";
    }

    std::string script(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    file.close();

    size_t bytecodeSize = 0;
    std::unique_ptr<char, void (*)(void*)> bytecode(luau_compile(script.c_str(), script.length(), nullptr, &bytecodeSize), free);
    if (!bytecode) {
        std::cout << "Failed to compile script" << scriptPath << "\n";
        std::cout << "Error: " << bytecode.get() << "\n";
        return "";
    }

    return std::string(bytecode.get(), bytecodeSize);
}

int main(int argc, char* argv[]) {    
    lua_State* L = luaL_newstate();
    if (!L)
    {
        std::cout << "Failed to create Lua state." << "\n";
    }

    luaL_openlibs(L);
    luaL_sandbox(L);

    if (!argv[1])
    {
        std::cout << "The first call argument cannot be NULL." << "\n";
        return 1;
    }

    std::string compilePath(argv[1]);
    std::string bytecode = compile_script(compilePath);

    int res = execute_luau_code(L, compilePath, bytecode);
    
    lua_close(L);

    return res;
}