#include <ssts/task_scheduler.hpp>
#include <thread>

#include <spdlog/spdlog.h>

#include "utils/utils.hpp"

int main() 
{
    ssts::utils::log(ssts::version());
    spdlog::set_pattern("[%H:%M:%S.%e] %v");

    ssts::task_scheduler s(8);
    s.start();

    ssts::utils::timer t;
    
    s.in("7", 7s, []{spdlog::info("7");});
    s.in("5", 5s, []{spdlog::info("5");});
    s.in("3a", 3s, []{spdlog::info("3a");});
    s.in("3b", 3s, []{spdlog::info("3b");});
    std::this_thread::sleep_for(1s);

    s.in("1a", 1s, []{spdlog::info("1a");});
    s.in("1b", 1s, []{spdlog::info("1b");});
    std::this_thread::sleep_for(7s);

    s.in(1s, []{spdlog::info("1c");});
    std::this_thread::sleep_for(4s);
    
    ssts::utils::log("Task Scheduler shutdown");
    s.stop();

    ssts::utils::log("Task Scheduler finished");
    return 0;
}
