#include "test_scheduler.hpp"

namespace ssts
{

class At : public SchedulerTest { };

TEST_F(At, FunctionOnly)
{
    InitScheduler(2u);
    s->at(ssts::clock::now() + 1s, []{std::cout << "(At) Hello!" << std::endl;});

    EXPECT_TRUE(get_size());
    Sleep(2s);
    EXPECT_FALSE(get_size());
}

TEST_F(At, FunctionParameters)
{
    InitScheduler(2u);
    s->at(ssts::clock::now() + 1s, [](auto p1, auto p2){std::cout << p1 << p2 <<std::endl;}, "(At) Input param:", 42);

    EXPECT_TRUE(get_size());
    Sleep(2s);
    EXPECT_FALSE(get_size());
}

TEST_F(At, TaskIdFunctionOnly)
{
    InitScheduler(2u);
    s->at("task_id", ssts::clock::now() + 1s, []{std::cout << "(At) Hello!" << std::endl;});

    EXPECT_TRUE(get_size());
    Sleep(2s); 
    EXPECT_FALSE(s->is_scheduled("task_id"s));
    EXPECT_FALSE(get_size());
}

TEST_F(At, TaskIdFunctionParameters)
{
    InitScheduler(2u);
    s->at("task_id", ssts::clock::now() + 1s, [](auto p1, auto p2){std::cout << p1 << p2 <<std::endl;}, "(At) Input param:", 42);

    EXPECT_TRUE(get_size());
    Sleep(2s); 
    EXPECT_FALSE(s->is_scheduled("task_id"s));
    EXPECT_FALSE(get_size());
}

TEST_F(At, TaskResultFunctionOnly)
{
    InitScheduler(2u);
    std::future f_at = s->at(ssts::clock::now() + 1s, []{ std::cout << "(At)" << std::endl; return 42; });

    EXPECT_TRUE(get_size());
    EXPECT_EQ(f_at.get(), 42);
    EXPECT_FALSE(get_size());
}

TEST_F(At, TaskResultFunctionParameters)
{
    InitScheduler(2u);
    std::future f_at = s->at(ssts::clock::now() + 1s, [](auto p){ std::cout << "(At)" << std::endl; return 2 * p; }, 21);

    EXPECT_TRUE(get_size());
    EXPECT_EQ(f_at.get(), 42);
    EXPECT_FALSE(get_size());
}

}
