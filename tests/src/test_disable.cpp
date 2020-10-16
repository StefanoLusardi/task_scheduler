#include "test_scheduler.hpp"

namespace ssts
{

class Disable : public SchedulerTest { };

TEST_F(Disable, Enabled)
{
    n_tasks = 3;
    InitScheduler(4u);
    StartAllTasksIn(2s);
    EXPECT_EQ(CountEnabledTasks(), n_tasks);
}

TEST_F(Disable, Disabled)
{
    n_tasks = 3;
    InitScheduler(4u);
    StartAllTasksIn(2s);

    // Disable tasks before they can run
    SetEnabledAllTasks(false);
    EXPECT_EQ(CountEnabledTasks(), 0u);
}

TEST_F(Disable, ReEnabled)
{
    n_tasks = 3;
    InitScheduler(4u);
    StartAllTasksIn(4s);

    // Disable tasks before they can run
    SetEnabledAllTasks(false);
    EXPECT_EQ(CountEnabledTasks(), 0u);

    // ReEnable tasks before they can run
    Sleep(1s);
    SetEnabledAllTasks(true);
    EXPECT_EQ(CountEnabledTasks(), n_tasks);
}

}
