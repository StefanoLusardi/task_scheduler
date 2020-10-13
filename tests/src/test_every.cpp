#include "test_scheduler.hpp"

namespace ssts
{

class Every : public SchedulerTest { };

TEST_F(Every, FunctionOnly)
{
    InitScheduler(2u);
    s->every(1s, []{std::cout << "(Every) Hello!" << std::endl;});

    EXPECT_EQ(s->size(), 1u);
    std::this_thread::sleep_for(2s);

    s->stop();
    EXPECT_EQ(s->size(), 0u);
}

TEST_F(Every, FunctionParameters)
{
    InitScheduler(2u);
    s->every(1s, [](auto p1, auto p2){std::cout << p1 << p2 << std::endl;}, "(Every) Input param:", 42);

    EXPECT_EQ(s->size(), 1u);
    std::this_thread::sleep_for(2s);

    s->stop();
    EXPECT_EQ(s->size(), 0u);
}

TEST_F(Every, TaskIdFunctionOnly)
{
    InitScheduler(2u);
    s->every("task_id", 1s, []{std::cout << "(Every) Hello!" << std::endl;});

    EXPECT_EQ(s->size(), 1u);
    std::this_thread::sleep_for(2s);

    s->stop();
    EXPECT_EQ(s->size(), 0u);
}

TEST_F(Every, TaskIdFunctionParameters)
{
    InitScheduler(2u);
    s->every("task_id", 1s, [](auto p1, auto p2){std::cout << p1 << p2 << std::endl;}, "(Every) Input param:", 42);

    EXPECT_EQ(s->size(), 1u);
    std::this_thread::sleep_for(2s);

    s->stop();
    EXPECT_EQ(s->size(), 0u);
}


TEST_F(Every, UpdateInterval) 
{
    InitScheduler(4u);
    StartAllTasksEvery(1s);
    EXPECT_EQ(s->size(), n_tasks);

    std::this_thread::sleep_for(2s);

    s->stop();
    EXPECT_EQ(s->size(), 0u);
}

}
