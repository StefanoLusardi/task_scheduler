#include <ssts/task_scheduler.hpp>

#include "utils/utils.hpp"

void t_update_interval() 
{
    ssts::utils::log_test("Update Interval");
    ssts::task_scheduler s(2);
    ssts::utils::timer t;

    s.every("task_id"s, 250ms, []{ std::cout << "Hi!" << std::endl; });

    ssts::utils::log("before update");
    std::this_thread::sleep_for(3s);

    s.update_interval("task_id"s, 1s);
    ssts::utils::log("after update");
    std::this_thread::sleep_for(4s);
}

int main() 
{
    ssts::utils::log(ssts::version());

    t_update_interval();

    ssts::utils::log("Task Scheduler finished");
    return 0;
}
