#pragma once 

#include "gtest/gtest.h"
#include <ssts/task_scheduler.hpp>

namespace ssts
{

class SchedulerTest : public ::testing::Test
{
protected:
    SchedulerTest() { }

    virtual ~SchedulerTest() { }

    virtual void SetUp() { }

    virtual void TearDown() { }

    void InitScheduler(const unsigned int n_threads)
    {
        s = std::make_unique<ssts::task_scheduler>(n_threads);
    }

    void StartAllTasksIn(ssts::clock::duration&& delay)
    {
        foreach_tasks([&](auto id){ s->in(std::string(id), std::move(delay), [id]{ std::cout << id << std::endl; }); });
    }

    void StartAllTasksEvery(ssts::clock::duration&& interval)
    {
        foreach_tasks([&](auto id) { s->every(std::string(id), std::move(interval), [id]{ std::cout << id << std::endl; }); });
    }

    void SetEnabledAllTasks(bool is_enabled)
    {
        foreach_tasks([&](auto id){ s->set_enabled(id, is_enabled); });
    }

    void RemoveAllTasks()
    {
        foreach_tasks([&](auto id){ s->remove_task(id); });
    }

    void UpdateAllTasksInterval(ssts::clock::duration&& interval)
    {
        foreach_tasks([&](auto id){ s->update_interval(id, interval); });
    }

    unsigned int CountScheduledTasks()
    {
        unsigned int count = 0;
        foreach_tasks([&](auto id)
        {
            if(s->is_scheduled(id))
                ++count;
        });

        return count;
    }

    unsigned int CountEnabledTasks()
    {
        unsigned int count = 0;
        foreach_tasks([&](auto id)
        {
            if(s->is_enabled(id))
                ++count;
        });

        return count;
    }

    std::unique_ptr<ssts::task_scheduler> s;
    unsigned int n_tasks = 4u;
    const std::string task_id_prefix = "task_id_"s;

private:
    template<typename Func, typename... Args>
    void foreach_tasks(Func&& func, Args&&... args)
    {
        for (auto n = 0; n < n_tasks; ++n)
        {
            std::invoke(std::forward<Func>(func), task_id_prefix + std::to_string(n), std::forward<Args>(args)...);
        }
    }
};

}
