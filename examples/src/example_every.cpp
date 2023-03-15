#include <ssts/task_scheduler.hpp>
#include <spdlog/spdlog.h>
#include "utils/utils.hpp"

int main() 
{
    spdlog::set_pattern("[%H:%M:%S.%e] %v");
    spdlog::info(ssts::version());

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
    
    spdlog::info("Task Scheduler shutdown");
    s.stop();

    spdlog::info("Task Scheduler finished");
    return 0;
}
