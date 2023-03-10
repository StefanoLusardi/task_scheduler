#include "scheduler_fixture.hpp"

namespace ssts
{

class Every : public SchedulerTest { };

TEST_F(Every, FunctionOnly)
{
    InitScheduler(2u);
    s->every(1s, []{std::cout << "(Every) Hello!" << std::endl;});

    EXPECT_EQ(get_size(), 1u);
    Sleep(2s);

    StopScheduler();
    EXPECT_EQ(get_size(), 0u);
}

TEST_F(Every, FunctionParameters)
{
    InitScheduler(2u);
    s->every(1s, [](auto p1, auto p2){std::cout << p1 << p2 << std::endl;}, "(Every) Input param:", 42);

    EXPECT_EQ(get_size(), 1u);
    Sleep(2s);

    StopScheduler();
    EXPECT_EQ(get_size(), 0u);
}

TEST_F(Every, TaskIdFunctionOnly)
{
    InitScheduler(2u);
    s->every("task_id", 1s, []{std::cout << "(Every) Hello!" << std::endl;});

    EXPECT_EQ(get_size(), 1u);
    Sleep(2s);

    StopScheduler();
    EXPECT_EQ(get_size(), 0u);
}

TEST_F(Every, TaskIdFunctionParameters)
{
    InitScheduler(2u);
    s->every("task_id", 1s, [](auto p1, auto p2){std::cout << p1 << p2 << std::endl;}, "(Every) Input param:", 42);

    EXPECT_EQ(get_size(), 1u);
    Sleep(2s);

    StopScheduler();
    EXPECT_EQ(get_size(), 0u);
}

TEST_F(Every, UpdateInterval) 
{
    InitScheduler(4u);
    StartAllTasksEvery(1s);
    Sleep(2s);

    UpdateAllTasksInterval(200ms);
    Sleep(2s);

    StopScheduler();
    EXPECT_EQ(get_size(), 0u);
}

}
