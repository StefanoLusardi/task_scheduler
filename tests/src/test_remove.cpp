#include "scheduler_fixture.hpp"

namespace ssts
{

class Remove : public SchedulerTest { };

TEST_F(Remove, ScheduledBeforeRun)
{
    n_tasks = 3u;
    InitScheduler(4u);
    StartAllTasksIn(1s);
    EXPECT_EQ(CountScheduledTasks(), n_tasks);
}

TEST_F(Remove, RemovedAfterRun)
{
    n_tasks = 3;
    InitScheduler(4u);
    StartAllTasksIn(1s);

    Sleep(2s);
    EXPECT_EQ(CountScheduledTasks(), 0u);
}

TEST_F(Remove, RemoveAll)
{
    n_tasks = 3;
    InitScheduler(4u);
    StartAllTasksIn(1s);
    RemoveAllTasks();
    EXPECT_EQ(CountScheduledTasks(), 0u);

    Sleep(2s);
    EXPECT_EQ(CountScheduledTasks(), 0u);
}


TEST_F(Remove, RestartAllAfterRemoveAll)
{
    n_tasks = 3;
    InitScheduler(4u);
    StartAllTasksIn(1s);
    RemoveAllTasks();
    EXPECT_EQ(CountScheduledTasks(), 0u);

    StartAllTasksIn(1s);
    EXPECT_EQ(CountScheduledTasks(), n_tasks);
}

TEST_F(Remove, RemovedLongTiming)
{
    n_tasks = 8;
    InitScheduler(4u);
    StartAllTasksIn(2min);

    EXPECT_EQ(CountScheduledTasks(), n_tasks);

    StopScheduler();
    EXPECT_EQ(CountScheduledTasks(), 0u);
}

TEST_F(Remove, FastAddRemoveLongerDuration)
{
    n_tasks = 16;
    auto task_id = "task_id_1"s;
    auto task_duration = 1s;
    auto task_sleep = 1s;

    InitScheduler(8u);
    StartAllTasksEvery(task_duration, task_sleep);

    s->remove_task(task_id);
    s->every(std::move(task_id), std::move(task_duration), [task_id, task_sleep]{ 
        std::cout << task_id << std::endl; 
        std::this_thread::sleep_for(task_sleep); 
    });
    Sleep(task_sleep);
    EXPECT_TRUE(s->is_scheduled(task_id));
    EXPECT_EQ(CountScheduledTasks(), n_tasks);
}

TEST_F(Remove, FastAddRemoveShorterDuration)
{
    n_tasks = 16;
    auto task_id = "task_id_1"s;
    auto task_duration = 10ms;

    InitScheduler(8u);
    StartAllTasksEvery(task_duration);

    s->remove_task(task_id);
    s->every(std::move(task_id), std::move(task_duration), [task_id]{ 
        std::cout << task_id << std::endl; 
    });
    Sleep(1s);
    EXPECT_TRUE(s->is_scheduled(task_id));
    EXPECT_EQ(CountScheduledTasks(), n_tasks);
}

}
