#include "test_scheduler.hpp"

namespace ssts
{

class Stop : public SchedulerTest { };

TEST_F(Stop, StopWhileRunningTasks)
{
    n_tasks = 32;
    InitScheduler(4u);
    StartAllTasksEvery(10ms, 2s);
    Sleep(1s);

    EXPECT_EQ(CountScheduledTasks(), n_tasks);

    Sleep(1s);
    StopScheduler();
    EXPECT_EQ(CountScheduledTasks(), 0u);
}


TEST_F(Stop, NoRunAllowedAfterSchedulerStop)
{
    n_tasks = 32;
    InitScheduler(4u);
    StartAllTasksEvery(10ms, 2s);

    Sleep(1s);
    StopScheduler();
    EXPECT_EQ(CountScheduledTasks(), 0u);
    
    StartAllTasksEvery(10ms, 2s);
    EXPECT_EQ(CountScheduledTasks(), 0u);
}

}