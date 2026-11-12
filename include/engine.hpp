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
#include "Luau/Compiler.h"

#include "custom_types.hpp"

inline std::filesystem::path GAME_ROOT;

enum class TaskPriority
{
    Default,
    Deferred
};

enum class TaskConcurrency
{
    Serial,
    Parallel
};

struct Task
{
    lua_State* thread;
    std::chrono::steady_clock::time_point time;
    TaskPriority priority;
    TaskConcurrency concurrency;
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

        void ScheduleTask(Task* task)
        {
            TaskScheduler.push_back(task);
        }

        std::string CompileScript(const std::string& scriptPath, Luau::CompileOptions options) const
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

            std::string bytecode = Luau::compile(script, options);

            if (bytecode.empty()) {
                std::cerr << "Failed to compile script" << scriptPath << "\n";
                std::cerr << "Error: " << bytecode << "\n";
                return "";
            }

            return bytecode;
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

        std::vector<Task*> TaskScheduler;
        lua_State* L;
};