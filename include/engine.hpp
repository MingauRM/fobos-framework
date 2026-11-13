#pragma once

#include "lua.h"
#include "luacode.h"
#include "luaconf.h"

#include "custom_types.hpp"

#include "task_scheduler.hpp"

class Engine
{
    private:
        Engine() = default;
        ~Engine() = default;

        TaskScheduler& task_scheduler = TaskScheduler::GetInstance();

    public:
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        static Engine& GetInstance()
        {
            static Engine instance;
            return instance;
        }

};