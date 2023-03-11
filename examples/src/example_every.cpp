#include <ssts/task_scheduler.hpp>
#include <thread>

#include <spdlog/spdlog.h>

#include "utils/utils.hpp"

int main() 
{
    ssts::utils::log(ssts::version());
    spdlog::set_pattern("[%H:%M:%S.%e] %v");

    ssts::task_scheduler s(8);
    s.set_duplicate_allowed(false);
    s.start();

    ssts::utils::timer t;

    s.every("task_1", 1s, []
    { 
        spdlog::info("Every 1s");
        std::this_thread::sleep_for(2500ms);
    });

    s.every("task_2"s, 2s, []
    { 
        spdlog::info("Every 2s");
        std::this_thread::sleep_for(3s);
    });

    s.every("task_3"s, 100ms, []
    { 
        spdlog::info("Every 3s");
        std::this_thread::sleep_for(900ms);
    });

    std::this_thread::sleep_for(10s);
    
    ssts::utils::log("Task Scheduler shutdown");
    s.stop();

    ssts::utils::log("Task Scheduler finished");
    return 0;
}
