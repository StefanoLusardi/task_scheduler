#include "gtest/gtest.h"
#include <ssts/task_scheduler.hpp>

TEST(sstsIn, FunctionOnly)
{
	ssts::task_scheduler s(2);
	s.in(1s, []{std::cout << "Hello!" << std::endl;});

    std::this_thread::sleep_for(2s);
	SUCCEED();
}

TEST(sstsIn, FunctionParameters)
{
	ssts::task_scheduler s(2);
	s.in(1s, [](auto p1, auto p2){std::cout << p1 << p2 <<std::endl;}, "Input param:", 42);

    std::this_thread::sleep_for(2s);
	SUCCEED();
}

TEST(sstsIn, TaskIdFunctionOnly)
{
	ssts::task_scheduler s(2);
	s.in("task_id", 1s, []{std::cout << "Hello!" << std::endl;});

    std::this_thread::sleep_for(2s);
	
	EXPECT_FALSE(s.is_scheduled("task_id"s));
}

TEST(sstsIn, TaskIdFunctionParameters)
{
	ssts::task_scheduler s(2);
	s.in("task_id", 1s, [](auto p1, auto p2){std::cout << p1 << p2 <<std::endl;}, "Input param:", 42);

    std::this_thread::sleep_for(2s);

	EXPECT_FALSE(s.is_scheduled("task_id"s));
}

TEST(sstsIn, TaskResultFunctionOnly)
{
	ssts::task_scheduler s(2);
	std::future f = s.in(1s, []{ return 42; });

	const auto result = f.get();
	EXPECT_EQ(result, 42);
}

TEST(sstsIn, TaskResultFunctionParameters)
{
	ssts::task_scheduler s(2);
	std::future f = s.in(1s, [](auto p){ return 2 * p; }, 21);

	const auto result = f.get();
	EXPECT_EQ(result, 42);
}
