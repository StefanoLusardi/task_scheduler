#include <ssts/task_scheduler.hpp>
#include "utils/utils.hpp"

ssts::task_scheduler s(4);

void t_recursive_bind(const std::string& str)
{
    std::cout << str << std::endl;
    s.in("bind", 1s, std::bind(&t_recursive_bind, str));
}

void t_recursive_lambda(const std::string& str)
{
    std::cout << str << std::endl;
    s.in("lambda", 1s, [str]{ t_recursive_lambda(str); });
}

int main()
{
    ssts::utils::log(ssts::version());
    ssts::utils::timer t;

    t_recursive_bind("BIND - I'm a Recursive Task!");
    t_recursive_lambda("LAMBDA - I'm a Recursive Task!");

    std::this_thread::sleep_for(5s);

    // s.remove_task("bind");
    // s.remove_task("lambda");

    ssts::utils::log("Task Scheduler finished");
    return 0;
}