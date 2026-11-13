#include "lua.h"
#include "luacode.h"
#include "luaconf.h"

#include <vector>

enum class TaskStatus
{
    Yielded,
    Running,
    Finished
};

enum class TaskConcurrency
{
    Inherited,
    Parallel,
    Serial
};

class Task
{
    private:
        lua_State* luauThread;
        TaskStatus status;
        TaskConcurrency concurrency = TaskConcurrency::Inherited;

        std::vector<Task> subTasks;

    public:
        ~Task() = default;
        Task(lua_State* thread, TaskStatus taskStatus, TaskConcurrency concurr = TaskConcurrency::Inherited)
        {
            luauThread = thread;
            status = taskStatus;
            concurrency = concurr;
        }

        TaskStatus GetStatus()
        {
            return status;
        }

        TaskConcurrency GetConcurrency()
        {
            return concurrency;
        }

};

class TaskScheduler
{
    private:
        TaskScheduler() = default;
        ~TaskScheduler() = default;

        std::vector<Task> tasks;

    public:
        TaskScheduler(const TaskScheduler&) = delete;
        TaskScheduler& operator=(const TaskScheduler&) = delete;

        static TaskScheduler& GetInstance()
        {
            static TaskScheduler instance;
            return instance;
        }
    
        void Schedule(Task task)
        {
            tasks.push_back(task);
        }

        void Update()
        {
            for (size_t i = 0; i < tasks.size(); i++)
            {
                if (tasks.at(i).GetStatus() == TaskStatus::Finished)
                {
                    tasks.erase(tasks.begin() + i);
                    i--;
                    continue;
                }

                


            }
        }
};