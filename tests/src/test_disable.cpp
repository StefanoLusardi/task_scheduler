#include "test_scheduler.hpp"

namespace ssts
{

class Disable : public SchedulerTest { };

TEST_F(Disable, Enabled)
{
    InitScheduler(4u);
    StartTasksIn(3u, 2s);

    EXPECT_TRUE(s->is_enabled("task_id_0"s));
    EXPECT_TRUE(s->is_enabled("task_id_1"s));
    EXPECT_TRUE(s->is_enabled("task_id_2"s));
}

TEST_F(Disable, Disabled)
{
    InitScheduler(4u);
    StartTasksIn(3u, 2s);

    // Disable tasks before they can run
    SetEnabledTasks(3u, false);
    EXPECT_FALSE(s->is_enabled("task_id_0"s));
    EXPECT_FALSE(s->is_enabled("task_id_1"s));
    EXPECT_FALSE(s->is_enabled("task_id_2"s));
}

TEST_F(Disable, ReEnabled)
{
    InitScheduler(4u);
    StartTasksIn(3u, 2s);

    // Disable tasks before they can run
    SetEnabledTasks(3u, false);
    EXPECT_FALSE(s->is_enabled("task_id_0"s));
    EXPECT_FALSE(s->is_enabled("task_id_1"s));
    EXPECT_FALSE(s->is_enabled("task_id_2"s));

    // ReEnable tasks before they can run
    SetEnabledTasks(3u, true);
    EXPECT_TRUE(s->is_enabled("task_id_0"s));
    EXPECT_TRUE(s->is_enabled("task_id_1"s));
    EXPECT_TRUE(s->is_enabled("task_id_2"s));
}

}
