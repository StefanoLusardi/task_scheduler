#include "test_scheduler.hpp"

namespace ssts
{

class Every : public SchedulerTest { };

TEST_F(Every, FunctionOnly)
{
    InitScheduler(2u);
    s->every(1s, []{std::cout << "Hello!" << std::endl;});

    EXPECT_NE(s->size(), 0);
    std::this_thread::sleep_for(2s);
    EXPECT_NE(s->size(), 0);

    // s->stop();
    // EXPECT_EQ(s->size(), 0);
}

TEST_F(Every, FunctionParameters)
{
    InitScheduler(2u);
    s->every(1s, [](auto p1, auto p2){std::cout << p1 << p2 << std::endl;}, "Input param:", 42);

    EXPECT_NE(s->size(), 0);
    std::this_thread::sleep_for(2s);
    EXPECT_NE(s->size(), 0);

    // s->stop();
    // EXPECT_EQ(s->size(), 0);
}

TEST_F(Every, TaskIdFunctionOnly)
{
    InitScheduler(2u);
    s->every("task_id", 1s, []{std::cout << "Hello!" << std::endl;});

    EXPECT_NE(s->size(), 0);
    std::this_thread::sleep_for(2s);
    EXPECT_NE(s->size(), 0);

    // s->stop();
    // EXPECT_EQ(s->size(), 0);
}

TEST_F(Every, TaskIdFunctionParameters)
{
    InitScheduler(2u);
    s->every("task_id", 1s, [](auto p1, auto p2){std::cout << p1 << p2 << std::endl;}, "Input param:", 42);

    EXPECT_NE(s->size(), 0);
    std::this_thread::sleep_for(2s);
    EXPECT_NE(s->size(), 0);

    // s->stop();
    // EXPECT_EQ(s->size(), 0);
}

}
