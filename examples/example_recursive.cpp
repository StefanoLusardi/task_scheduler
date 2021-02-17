#include <ssts/task_scheduler.hpp>
#include "utils/utils.hpp"

ssts::task_scheduler s(4);

void t_recursive();

void recursive(const std::string& str)
{
    std::cout << str << std::endl;
    t_recursive();
}

void t_recursive()
{
    s.in("RecursiveTask", 1s, std::bind(&recursive, "I'm a Recursive Task!"));
    // while(!s.is_scheduled("RecursiveTask"))
    //     std::this_thread::sleep_for(10ms);
    
    // std::cout << std::boolalpha << s.is_scheduled("RecursiveTask") << std::endl;
}

int main()
{
    ssts::utils::log(ssts::version());
    ssts::utils::timer t;

    t_recursive();

    std::this_thread::sleep_for(20s);
    ssts::utils::log("Task Scheduler finished");
    return 0;
}