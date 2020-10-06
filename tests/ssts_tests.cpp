#include "gtest/gtest.h"
#include <ssts/task_scheduler.hpp>

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	std::cout << ssts::version() << std::endl;
	return RUN_ALL_TESTS();
}