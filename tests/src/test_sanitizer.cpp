#include "test_scheduler.hpp"

#include <mutex>
#include <atomic>
#include <iostream>
#include <thread>

namespace ssts
{

TEST(Sanitizer, ThreadSanitizer)
{
    std::mutex mutex;
    int a = 3;
    const size_t size = 1000 * 1000;
    std::atomic<int> b(1);
    
    std::thread t1([&]
    {
        for (size_t counter = 0; counter < size; counter++)
        {
            ++b;
            // std::unique_lock<std::mutex> lock(mutex);
            ++a;
        }
    });
    
    std::thread t2([&]
    {
        for (size_t counter = 0; counter < size; counter++)
        {
            --b;
            // std::unique_lock<std::mutex> lock(mutex);
            --a;
        }
    });

    t1.join();
    t2.join();
}

}
