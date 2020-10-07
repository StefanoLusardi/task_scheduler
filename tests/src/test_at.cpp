#include "gtest/gtest.h"
#include <ssts/task_scheduler.hpp>

TEST(sstsAt, FunctionOnly)
{
    ssts::task_scheduler s(2);
    s.at(ssts::clock::now() + 1s, []{std::cout << "Hello!" << std::endl;});

    std::this_thread::sleep_for(2s);
    SUCCEED();
}

TEST(sstsAt, FunctionParameters)
{
    ssts::task_scheduler s(2);
    s.at(ssts::clock::now() + 1s, [](auto p1, auto p2){std::cout << p1 << p2 <<std::endl;}, "Input param:", 42);

    std::this_thread::sleep_for(2s);
    SUCCEED();
}

TEST(sstsAt, TaskIdFunctionOnly)
{
    ssts::task_scheduler s(2);
    s.at("task_id", ssts::clock::now() + 1s, []{std::cout << "Hello!" << std::endl;});

    std::this_thread::sleep_for(2s);
    
    EXPECT_FALSE(s.is_scheduled("task_id"s));
}

TEST(sstsAt, TaskIdFunctionParameters)
{
    ssts::task_scheduler s(2);
    s.at("task_id", ssts::clock::now() + 1s, [](auto p1, auto p2){std::cout << p1 << p2 <<std::endl;}, "Input param:", 42);

    std::this_thread::sleep_for(2s);
    
    EXPECT_FALSE(s.is_scheduled("task_id"s));
}

TEST(sstsAt, TaskResultFunctionOnly)
{
    ssts::task_scheduler s(2);
    std::future f = s.at(ssts::clock::now() + 1s, []{ return 42; });

    const auto result = f.get();
    EXPECT_EQ(result, 42);
}

TEST(sstsAt, TaskResultFunctionParameters)
{
    ssts::task_scheduler s(2);
    std::future f = s.at(ssts::clock::now() + 1s, [](auto p){ return 2 * p; }, 21);

    const auto result = f.get();
    EXPECT_EQ(result, 42);
}
