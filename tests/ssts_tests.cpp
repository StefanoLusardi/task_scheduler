#include "gtest/gtest.h"
#include <ssts/task_scheduler.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    std::cout << ssts::version() << std::endl;

    // ::testing::GTEST_FLAG(filter) = "Every.*";

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
