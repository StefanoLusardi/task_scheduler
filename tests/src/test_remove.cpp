#include "test_scheduler.hpp"

namespace ssts
{

class Remove : public SchedulerTest { };

TEST_F(Remove, ScheduledBeforeRun)
{
    InitScheduler(4u);
    StartTasksIn(3u, 1s);

    EXPECT_TRUE(s->is_scheduled("task_id_0"s));
    EXPECT_TRUE(s->is_scheduled("task_id_1"s));
    EXPECT_TRUE(s->is_scheduled("task_id_2"s));
}

TEST_F(Remove, RemovedAfterRun)
{
    InitScheduler(4u);
    StartTasksIn(3u, 1s);

    std::this_thread::sleep_for(2s);

    EXPECT_FALSE(s->is_scheduled("task_id_0"s));
    EXPECT_FALSE(s->is_scheduled("task_id_1"s));
    EXPECT_FALSE(s->is_scheduled("task_id_2"s));
}

TEST_F(Remove, RemovedBeforeAfterSleep)
{
    InitScheduler(4u);
    StartTasksIn(3u, 1s);
    RemoveTasks(3u);

    EXPECT_TRUE(s->is_scheduled("task_id_0"s));
    EXPECT_TRUE(s->is_scheduled("task_id_1"s));
    EXPECT_TRUE(s->is_scheduled("task_id_2"s));

    std::this_thread::sleep_for(2s);

    EXPECT_FALSE(s->is_scheduled("task_id_0"s));
    EXPECT_FALSE(s->is_scheduled("task_id_1"s));
    EXPECT_FALSE(s->is_scheduled("task_id_2"s));
}

}
