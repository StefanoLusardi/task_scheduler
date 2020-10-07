#include "test_scheduler.hpp"

namespace ssts
{

class At : public SchedulerTest { };

TEST_F(At, FunctionOnly)
{
    InitScheduler(2u);
    s->at(ssts::clock::now() + 1s, []{std::cout << "Hello!" << std::endl;});

    EXPECT_TRUE(s->size());
    std::this_thread::sleep_for(2s);
    EXPECT_FALSE(s->size());
}

TEST_F(At, FunctionParameters)
{
    InitScheduler(2u);
    s->at(ssts::clock::now() + 1s, [](auto p1, auto p2){std::cout << p1 << p2 <<std::endl;}, "Input param:", 42);

    EXPECT_TRUE(s->size());
    std::this_thread::sleep_for(2s);
    EXPECT_FALSE(s->size());
}

TEST_F(At, TaskIdFunctionOnly)
{
    InitScheduler(2u);
    s->at("task_id", ssts::clock::now() + 1s, []{std::cout << "Hello!" << std::endl;});

    EXPECT_TRUE(s->size());
    std::this_thread::sleep_for(2s); 
    EXPECT_FALSE(s->is_scheduled("task_id"s));
}

TEST_F(At, TaskIdFunctionParameters)
{
    InitScheduler(2u);
    s->at("task_id", ssts::clock::now() + 1s, [](auto p1, auto p2){std::cout << p1 << p2 <<std::endl;}, "Input param:", 42);

    EXPECT_TRUE(s->size());
    std::this_thread::sleep_for(2s); 
    EXPECT_FALSE(s->is_scheduled("task_id"s));
}

TEST_F(At, TaskResultFunctionOnly)
{
    InitScheduler(2u);
    std::future f = s->at(ssts::clock::now() + 1s, []{ return 42; });

    EXPECT_TRUE(s->size());
    EXPECT_EQ(f.get(), 42);
    EXPECT_FALSE(s->size());
}

TEST_F(At, TaskResultFunctionParameters)
{
    InitScheduler(2u);
    std::future f = s->at(ssts::clock::now() + 1s, [](auto p){ return 2 * p; }, 21);

    EXPECT_TRUE(s->size());
    EXPECT_EQ(f.get(), 42);
    EXPECT_FALSE(s->size());
}

}
