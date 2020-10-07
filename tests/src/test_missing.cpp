#include "test_scheduler.hpp"

namespace ssts
{

class Missing : public SchedulerTest { };

TEST_F(Missing, NoScheduledId)
{
    InitScheduler(4u);
    StartTasksIn(4u, 2s);
    EXPECT_FALSE(s->is_scheduled("missing_id"s));
}

TEST_F(Missing, NoIsEnabledId)
{
    InitScheduler(4u);
    StartTasksIn(4u, 2s);
    EXPECT_FALSE(s->is_enabled("missing_id"s));
}

TEST_F(Missing, NoSetDisabledId)
{
    InitScheduler(4u);
    StartTasksIn(4u, 2s);
    EXPECT_FALSE(s->set_enabled("missing_id"s, true));
}

TEST_F(Missing, NoRemoveTaskId)
{
    InitScheduler(4u);
    StartTasksIn(4u, 2s);
    EXPECT_FALSE(s->remove_task("missing_id"s));
}

TEST_F(Missing, NoRunningTaskAfterStop)
{
    InitScheduler(4u);
    s->stop();
    StartTasksIn(4u, 2s);
    EXPECT_FALSE(s->size());
}

// TEST_F(Missing, NoRunningTaskAfterForceStop)
// {
//     InitScheduler(4u);
//     StartTasksIn(4u, 2s);
//     EXPECT_TRUE(s->size());
//     s->stop(force=true); //TODO!
//     StartTasksIn(4u, 2s);
//     EXPECT_FALSE(s->size());
// }

}
