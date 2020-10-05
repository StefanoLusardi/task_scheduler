#include "gtest/gtest.h"
#include <ssts/task_scheduler.hpp>

TEST(sstsEvery, FunctionOnly)
{
	ssts::task_scheduler s(2);
	s.every(1s, 
		[]{std::cout << "Hello!" << std::endl;});

    std::this_thread::sleep_for(2s);
	SUCCEED();
}

TEST(sstsEvery, FunctionParameters)
{
	ssts::task_scheduler s(2);
	s.every(1s, 
		[](auto p1, auto p2){std::cout << p1 << p2 <<std::endl;}, 
		"Input param:", 
		42);

    std::this_thread::sleep_for(2s);
	SUCCEED();
}

TEST(sstsEvery, TaskIdFunctionOnly)
{
	ssts::task_scheduler s(2);
	s.every("task_id", 
		1s, 
		[]{std::cout << "Hello!" << std::endl;});

    std::this_thread::sleep_for(2s);
	SUCCEED();
}

TEST(sstsEvery, TaskIdFunctionParameters)
{
	ssts::task_scheduler s(2);
	s.every("task_id", 
		1s, 
		[](auto p1, auto p2){std::cout << p1 << p2 <<std::endl;}, 
		"Input param:", 
		42);

    std::this_thread::sleep_for(2s);
	SUCCEED();
}
