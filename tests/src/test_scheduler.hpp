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

    void StartTasksIn(const unsigned int n_tasks, ssts::clock::duration&& delay)
    {
        const auto task_id = "task_id_"s;
        for (auto n = 0; n < n_tasks; ++n)
        {
            auto id = std::string(task_id + std::to_string(n));
            s->in(std::string(id), std::move(delay), [str_id = std::string(id)]{ std::cout << str_id << std::endl; });
        }
    }

    void SetEnabledTasks(const unsigned int n_tasks, bool is_enabled)
    {
        const auto task_id = "task_id_"s;
        for (auto n = 0; n < n_tasks; ++n)
        {
            auto id = std::string(task_id + std::to_string(n));
            s->set_enabled(id, is_enabled);
        }
    }

    void RemoveTasks(const unsigned int n_tasks)
    {
        const auto task_id = "task_id_"s;
        for (auto n = 0; n < n_tasks; ++n)
        {
            auto id = std::string(task_id + std::to_string(n));
            s->remove_task(id);
        }
    }

    std::unique_ptr<ssts::task_scheduler> s;
};

}
