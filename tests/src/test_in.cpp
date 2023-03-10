#include "scheduler_fixture.hpp"

namespace ssts
{

class In : public SchedulerTest { };

TEST_F(In, FunctionOnly)
{
    InitScheduler(2u);
    s->in(1s, []{std::cout << "(In) Hello!" << std::endl;});

    EXPECT_TRUE(get_size());
    Sleep(2s);
    EXPECT_FALSE(get_size());
}

TEST_F(In, FunctionParameters)
{
    InitScheduler(2u);
    s->in(1s, [](auto p1, auto p2){std::cout << p1 << p2 << std::endl;}, "(In) Input param:", 42);

    EXPECT_TRUE(get_size());
    Sleep(2s);
    EXPECT_FALSE(get_size());
}

TEST_F(In, TaskIdFunctionOnly)
{
    InitScheduler(2u);
    s->in("task_id", 1s, []{std::cout << "(In) Hello!" << std::endl;});

    EXPECT_TRUE(get_size());
    Sleep(2s);    
    EXPECT_FALSE(s->is_scheduled("task_id"s));
}

TEST_F(In, TaskIdFunctionParameters)
{
    InitScheduler(2u);
    s->in("task_id", 1s, [](auto p1, auto p2){std::cout << p1 << p2 <<std::endl;}, "(In) Input param:", 42);

    EXPECT_TRUE(get_size());
    Sleep(2s);
    EXPECT_FALSE(s->is_scheduled("task_id"s));
}

TEST_F(In, TaskResultFunctionOnly)
{
    InitScheduler(2u);
    std::future f_in = s->in(1s, []{ std::cout << "(In)" << std::endl; return 42; });

    EXPECT_TRUE(get_size());
    EXPECT_EQ(f_in.get(), 42);
    EXPECT_FALSE(get_size());
}

TEST_F(In, TaskResultFunctionParameters)
{
    InitScheduler(2u);
    std::future f_in = s->in(1s, [](auto p){ std::cout << "(In)" << std::endl; return 2 * p; }, 21);

    EXPECT_TRUE(get_size());
    EXPECT_EQ(f_in.get(), 42);
    EXPECT_FALSE(get_size());
}

}
