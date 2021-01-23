#include "gtest/gtest.h"
#include <ssts/task_scheduler.hpp>

namespace ssts
{

TEST(Pool, ImmediateStop)
{
    auto tp = ssts::task_pool(8);
    tp.stop();
}

TEST(Pool, LessTasksThenThreadSize)
{
    auto tp = std::make_unique<ssts::task_pool>(4);
    tp->run([]{ std::cout << "task pool is doing some work" << std::endl; });
    tp->stop();
}

TEST(Pool, MoreTasksThenThreadSize)
{
    auto tp = std::make_unique<ssts::task_pool>(2);
    tp->run([]{ std::cout << "1) task pool is doing some work" << std::endl; std::this_thread::sleep_for(2s); });
    tp->run([]{ std::cout << "2) task pool is doing some work" << std::endl; std::this_thread::sleep_for(2s); });
    tp->run([]{ std::cout << "3) task pool is doing some work" << std::endl; std::this_thread::sleep_for(2s); });
    tp->stop();
}

}
