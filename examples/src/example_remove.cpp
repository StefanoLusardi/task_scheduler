#include <ssts/task_scheduler.hpp>
#include <spdlog/spdlog.h>
#include "utils/utils.hpp"

int main() 
{
    spdlog::set_pattern("[%H:%M:%S.%e] %v");
    spdlog::info(ssts::version());

    ssts::task_scheduler s(8);
    s.start();
    
    ssts::utils::timer t;
    
    s.in("Foo"s, 2s, []{ spdlog::info("Foo"); });
    s.in("Bar"s, 3s, []{ spdlog::info("Bar"); });

    spdlog::info("before remove");
    std::this_thread::sleep_for(2s);

    s.remove_task("foo"s); // Wrong task ID
    s.remove_task("Bar"s);

    spdlog::info("after remove");
    std::this_thread::sleep_for(2s);

    spdlog::info("Task Scheduler shutdown");
    s.stop();

    spdlog::info("Task Scheduler finished");
    return 0;
}
