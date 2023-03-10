#include <ssts/task_scheduler.hpp>
#include "utils/utils.hpp"

void t_nested_in()
{
    ssts::utils::log_test("Nested In");

    ssts::task_scheduler s(2);

    ssts::utils::timer t;

    s.in(2s, 
    [&s]{ 
        std::cout << "First!" << std::endl;
        s.in(2s, []{ std::cout << "Second!" << std::endl; });
    });

    std::this_thread::sleep_for(5s);
}

void t_nested_every()
{
    ssts::utils::log_test("Nested Every");

    ssts::task_scheduler s(3);

    ssts::utils::timer t;

    bool is_inner_enabled = true;
    bool is_inner_removed = false;

    s.every(
        "main_task_id"s, 
        1s, 
        [&s](bool is_inner_enabled, bool is_inner_removed)
        { 
            std::cout << "Main Task running on thread: " << std::this_thread::get_id() << std::endl;

            s.in("inner_task_id"s, 10ms, []()
            { 
                std::cout << "Inner Task running on thread: " << std::this_thread::get_id() << std::endl; 
            });

            s.set_enabled("inner_task_id", is_inner_enabled);
            if(is_inner_removed)
                s.remove_task("inner_task_id");
        }, 
        std::ref(is_inner_enabled), 
        std::ref(is_inner_removed)
    );

    ssts::utils::log("run nested");
    std::this_thread::sleep_for(5s);

    ssts::utils::log("disabled main_task_id");
    s.set_enabled("main_task_id", false);
    std::this_thread::sleep_for(5s);

    ssts::utils::log("enabled main_task_id");
    s.set_enabled("main_task_id", true);
    std::this_thread::sleep_for(5s);

    ssts::utils::log("disabled inner_task_id");
    is_inner_enabled = false;
    std::this_thread::sleep_for(5s);

    ssts::utils::log("enabled inner_task_id");
    is_inner_enabled = true;
    std::this_thread::sleep_for(5s);

    ssts::utils::log("removed inner_task_id");
    is_inner_removed = true;
    std::this_thread::sleep_for(5s);

    ssts::utils::log("removed main_task_id");
    s.remove_task("main_task_id");
    std::this_thread::sleep_for(5s);

    s.stop();
}

int main()
{
    ssts::utils::log(ssts::version());

    t_nested_in();
    t_nested_every();
    
    ssts::utils::log("Task Scheduler finished");
    return 0;
}