#include "test_scheduler.hpp"

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

    std::this_thread::sleep_for(2s);
    EXPECT_EQ(CountScheduledTasks(), 0u);
}

TEST_F(Remove, RemovedBeforeAfterSleep)
{
    n_tasks = 3;
    InitScheduler(4u);
    StartAllTasksIn(1s);
    RemoveAllTasks();
    EXPECT_EQ(CountScheduledTasks(), n_tasks);

    std::this_thread::sleep_for(2s);
    EXPECT_EQ(CountScheduledTasks(), 0u);
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

}
