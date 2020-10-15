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

    virtual void SetUp() override { }

    virtual void TearDown() override { }

    void InitScheduler(const unsigned int n_threads)
    {
        log("Initialing Scheduler");
        s = std::make_unique<ssts::task_scheduler>(n_threads);
        log("Scheduler Initialized with", n_threads, " threads");
    }

    void StopScheduler() 
    {
        log("Stopping Scheduler");
        if(!s)
        {
            log("Scheduler already stopped.");
            return;
        }

        s->stop();
        log("Scheduler Stopped");
    }

    void Sleep(ssts::clock::duration&& delay) 
    {
        const auto scaled_delay = scale_duration(delay);

        log("Sleeping for", get_seconds(scaled_delay), "seconds");        
        std::this_thread::sleep_for(scaled_delay);
        log("Slept for", get_seconds(scaled_delay), "seconds");
    }

    void StartAllTasksIn(ssts::clock::duration&& delay)
    {
        log("Scheduling", n_tasks, "tasks", "using in() API");
        foreach_tasks([&](auto id){ s->in(std::string(id), std::move(delay), [id]{ std::cout << id << std::endl; }); });
        log("Scheduled", n_tasks, "tasks - in", get_seconds(delay), "seconds");
    }

    void StartAllTasksEvery(ssts::clock::duration&& interval)
    {
        log("Scheduling", n_tasks, "tasks", "using every()) API");
        foreach_tasks([&](auto id) { s->every(std::string(id), std::move(interval), [id]{ std::cout << id << std::endl; }); });
        log("Scheduled", n_tasks, "tasks - every", get_seconds(interval), "seconds");
    }

    void SetEnabledAllTasks(bool is_enabled)
    {
        log("Enabling", n_tasks, "tasks");
        foreach_tasks([&](auto id){ s->set_enabled(id, is_enabled); });
        log("Enabled", n_tasks, "tasks");
    }

    void RemoveAllTasks()
    {
        log("Removing", n_tasks, "tasks");
        foreach_tasks([&](auto id){ s->remove_task(id); });
        log("Removed", n_tasks, "tasks");
    }

    void UpdateAllTasksInterval(ssts::clock::duration&& interval)
    {
        log("Updating", n_tasks, "tasks");
        foreach_tasks([&](auto id){ s->update_interval(id, interval); });
        log("Updated", n_tasks, "tasks - interval", get_seconds(interval), "seconds");
    }

    unsigned int CountScheduledTasks()
    {
        unsigned int count = 0;
        log("Counting Scheduled Tasks");
        foreach_tasks([&](auto id)
        {
            if(s->is_scheduled(id))
                ++count;
        });
        log("Counted Scheduled Tasks: ", count, "tasks scheduled");

        return count;
    }

    unsigned int CountEnabledTasks()
    {
        unsigned int count = 0;
        log("Counting Enabled Tasks");
        foreach_tasks([&](auto id)
        {
            if(s->is_enabled(id))
                ++count;
        });
        log("Counted Enabled Tasks: ", count, "tasks enabled");

        return count;
    }

    std::unique_ptr<ssts::task_scheduler> s;
    unsigned int n_tasks = 4u;
    const std::string task_id_prefix = "task_id_"s;
    unsigned int sleep_duration_scale_factor = 3u;

private:
    template<typename Func, typename... Args>
    void foreach_tasks(Func&& func, Args&&... args)
    {
        for (auto n = 0; n < n_tasks; ++n)
        {
            std::invoke(std::forward<Func>(func), task_id_prefix + std::to_string(n), std::forward<Args>(args)...);
        }
    }

    template<typename... Args>
    void log(Args&&... args)
    {
        ((std::cout << std::forward<Args>(args) << ' ') , ...) << "\n" << std::endl;
    }

    template<typename T>
    auto scale_duration(T&& time_duration)
    { 
        return time_duration * sleep_duration_scale_factor;
    }
    
    unsigned int get_seconds(const ssts::clock::duration& delay)
    {
        return std::chrono::duration_cast<std::chrono::seconds>(delay).count();
    }
};

}
