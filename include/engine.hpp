#pragma once

#include <vector>
#include <chrono>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <ios>

#include "lua.h"
#include "lualib.h"
#include "luaconf.h"
#include "luacode.h"

inline std::filesystem::path GAME_ROOT;

enum class TaskType
{
    Default,
    Deferred
};

struct Task
{
    lua_State* thread;
    TaskType type;
};

class Engine
{
    public:
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(Engine&&) = delete;

        static Engine& GetInstance()
        {
            static Engine instance; 
            return instance;
        }

        void UpdateScheduler()
        {

        }

        void ScheduleTask(Task task, std::chrono::duration<double> waitTime)
        {
            
        }

        std::string CompileScript(const std::string& scriptPath) const
        {
            std::ifstream file(scriptPath, std::ios::in | std::ios::binary);
            if (!file)
            {
                std::cerr << "Could not open script file: " << scriptPath << "\n";
                return "";
            }

            std::string script(
                (std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>()
            );
            file.close();

            size_t bytecodeSize = 0;
            char* bytecode = luau_compile(script.c_str(), script.length(), nullptr, &bytecodeSize);

            if (!bytecode) {
                std::cerr << "Failed to compile script" << scriptPath << "\n";
                std::cerr << "Error: " << bytecode << "\n";
                return "";
            }

            std::string bytecodeString = std::string(bytecode, bytecodeSize);
            free(bytecode);

            return bytecodeString;
        }

        int ExecuteBytecode(const std::string& chunkName, const std::string& bytecode) const
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

        lua_State* GetLuaState() const
        {
            return L;
        }

    private:
        Engine()
        {
            L = luaL_newstate();
            if (!L)
            {
                std::cerr << "Failed to create Lua state." << "\n";
            }

            luaL_openlibs(L);
            luaL_sandbox(L);
        }

        ~Engine() = default;

        std::vector<lua_State*> TaskScheduler;
        lua_State* L;
};