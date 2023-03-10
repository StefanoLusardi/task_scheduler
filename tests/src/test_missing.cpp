#include "scheduler_fixture.hpp"

namespace ssts
{

class Missing : public SchedulerTest { };

TEST_F(Missing, NoScheduledId)
{
    InitScheduler(4u);
    StartAllTasksIn(2s);
    EXPECT_FALSE(s->is_scheduled("missing_id"s));
}

TEST_F(Missing, NoIsEnabledId)
{
    InitScheduler(4u);
    StartAllTasksIn(2s);
    EXPECT_FALSE(s->is_enabled("missing_id"s));
}

TEST_F(Missing, NoSetDisabledId)
{
    InitScheduler(4u);
    StartAllTasksIn(2s);
    EXPECT_FALSE(s->set_enabled("missing_id"s, true));
}

TEST_F(Missing, NoRemoveTaskId)
{
    InitScheduler(4u);
    StartAllTasksIn(2s);
    EXPECT_FALSE(s->remove_task("missing_id"s));
}

TEST_F(Missing, NoRunningTaskAfterImmediateStop)
{
    InitScheduler(4u);
    StopScheduler();
    StartAllTasksIn(2s);
    EXPECT_EQ(get_size(), 0u);
}

TEST_F(Missing, NoRunningTaskAfterLaterStop)
{
    InitScheduler(4u);
    StartAllTasksIn(1s);
    EXPECT_EQ(get_size(), 4u);

    Sleep(1s);

    StopScheduler();
    StartAllTasksIn(1s);
    EXPECT_EQ(get_size(), 0u);
}

TEST_F(Missing, NoUpdateIntervalMissingId)
{
    InitScheduler(4u);
    StartAllTasksEvery(1s);

    EXPECT_FALSE(s->update_interval("missing_id"s, 2s));
    EXPECT_EQ(get_size(), 4u);
}

TEST_F(Missing, NoUpdateIntervalIn)
{
    InitScheduler(4u);
    s->in("task_id", 1s, []{std::cout << "Missing (In)" << std::endl;});

    EXPECT_FALSE(s->update_interval("task_id"s, 2s));
    EXPECT_EQ(get_size(), 1u);
}

TEST_F(Missing, NoUpdateIntervalAt)
{
    InitScheduler(4u);
    s->at("task_id", ssts::clock::now() + 1s, []{std::cout << "Missing (At)" << std::endl;});

    EXPECT_FALSE(s->update_interval("task_id"s, 2s));
    EXPECT_EQ(get_size(), 1u);
}

}
