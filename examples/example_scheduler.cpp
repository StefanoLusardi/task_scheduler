#include <ssts/task_scheduler.hpp>
#include "utils/utils.hpp"

void t_simple()
{
    ssts::utils::log_test("Simple");

    ssts::task_scheduler s(2);

    ssts::utils::timer t;

    // Task without parameters
    s.in(5s, []{std::cout << "Hello!" << std::endl;});

    // Task with parameters
    s.at(ssts::clock::now() + 1s, 
    [](auto str1, auto str2) { 
        std::cout << "Multi Params: " << str2 << ", " << str1 << std::endl; 
    }, 
    42, 
    "fourty-two");

    std::this_thread::sleep_for(3s);
}

void t_return_value()
{
    ssts::utils::log_test("Return value");

    ssts::task_scheduler s(2);
    
    ssts::utils::timer t;

    // Task with return value (not retrived)
    s.in(3s, 
    [](auto str){ 
        std::cout << "The number: " << str << std::endl; 
        return "HEY!"s; 
    }, 
    42);

    // Task with return value (retrieved using std::future)
    std::future f = s.in(1s, 
    []{ 
        std::cout << "Wait for my result" << std::endl;
        std::this_thread::sleep_for(2s);
        return "This-is-the-result"s; 
    });

    std::cout << "\nWaiting task result" << std::endl;
    std::cout << "Task result: " << f.get() << std::endl;
}

void t_task_pool_size_ok()
{
    /*
    Every second, run a task that lasts 2 seconds. 
    Here using 4 threads is enough to keep the workload on time.
    Adjust number of thread (ssts::task_scheduler constructor parameter) to change thread pool size and run more tasks in parallel.
    */
    
    ssts::utils::log_test("Task pool size - OK");

    ssts::task_scheduler s(8);

    ssts::utils::timer t;

    s.every(100ms, []{std::cout << "Hello A!" << std::endl; std::this_thread::sleep_for(2s);});
    s.every(100ms, []{std::cout << "Hello B!" << std::endl; std::this_thread::sleep_for(2s);});
    s.every(100ms, []{std::cout << "Hello C!" << std::endl; std::this_thread::sleep_for(2s);});
    s.every(100ms, []{std::cout << "Hello D!" << std::endl; std::this_thread::sleep_for(2s);});
    s.every(100ms, []{std::cout << "Hello E!" << std::endl; std::this_thread::sleep_for(2s);});
    s.every(100ms, []{std::cout << "Hello F!" << std::endl; std::this_thread::sleep_for(2s);});

    std::this_thread::sleep_for(5s);
}

void t_task_pool_size_too_small()
{
    /*
    Every second, run a task that lasts 2 seconds. 
    Here using only 2 threads is not enough to keep the workload on time.
    Adjust number of thread (ssts::task_scheduler constructor parameter) to change thread pool size and run more tasks in parallel.
    */

    ssts::utils::log_test("Task pool size - Too small");
    
    ssts::task_scheduler s(2);

    ssts::utils::timer t;

    s.every(100ms, []{std::cout << "Hello A!" << std::endl; std::this_thread::sleep_for(2s);});
    s.every(100ms, []{std::cout << "Hello B!" << std::endl; std::this_thread::sleep_for(2s);});
    s.every(100ms, []{std::cout << "Hello C!" << std::endl; std::this_thread::sleep_for(2s);});
    s.every(100ms, []{std::cout << "Hello D!" << std::endl; std::this_thread::sleep_for(2s);});
    s.every(100ms, []{std::cout << "Hello E!" << std::endl; std::this_thread::sleep_for(2s);});
    s.every(100ms, []{std::cout << "Hello F!" << std::endl; std::this_thread::sleep_for(2s);});

    std::this_thread::sleep_for(10s);
}

void t_in()
{
    /// task_scheduler::in() APIs
    ssts::utils::log_test("task_scheduler::in() APIs");
    
    const unsigned n_tasks { 3 };
    ssts::task_scheduler s(n_tasks);

    ssts::utils::timer t;
    
    const auto wait_time_pre = 1s;
    const auto wait_time_post = 3s;

    // TaskFunction only (no task_id, no parameters)
    s.in(2s, []{std::cout << "Hello A!" << std::endl;});
    ssts::utils::check_tasks("pre", s, n_tasks, wait_time_pre);
    ssts::utils::check_tasks("post", s, n_tasks, wait_time_post);

    // TaskFunction with parameters (no task_id)
    s.in(2s, [](auto p){std::cout << "Hello B! " << p << std::endl;}, "(with parameters)");
    ssts::utils::check_tasks("pre", s, n_tasks, wait_time_pre);
    ssts::utils::check_tasks("post", s, n_tasks, wait_time_post);

    // TaskFunction with parameters and task_id
    s.in("task_id_3"s, 2s, [](auto p){std::cout << "Hello C! " << p << std::endl;}, "(with parameters)");
    ssts::utils::check_tasks("pre", s, n_tasks, wait_time_pre);
    ssts::utils::check_tasks("post", s, n_tasks, wait_time_post);
}

void t_at()
{
    /// task_scheduler::at() APIs
    ssts::utils::log_test("task_scheduler::at() APIs");

    const unsigned n_tasks { 3 };
    ssts::task_scheduler s(n_tasks);
    
    ssts::utils::timer t;
    
    const auto wait_time_pre = 1s;
    const auto wait_time_post = 3s;

    // TaskFunction only (no task_id, no parameters)
    s.at(ssts::clock::now() + 2s, []{std::cout << "Hello A!" << std::endl;});
    ssts::utils::check_tasks("pre", s, n_tasks, wait_time_pre);
    ssts::utils::check_tasks("post", s, n_tasks, wait_time_post);

    // TaskFunction with parameters (no task_id)
    s.at(ssts::clock::now() + 2s, [](auto p){std::cout << "Hello B! " << p << std::endl;}, "(with parameters)");
    ssts::utils::check_tasks("pre", s, n_tasks, wait_time_pre);
    ssts::utils::check_tasks("post", s, n_tasks, wait_time_post);

    // TaskFunction with parameters and task_id
    s.at("task_id_3"s, ssts::clock::now() + 2s, [](auto p){std::cout << "Hello C! " << p << std::endl;}, "(with parameters)");
    ssts::utils::check_tasks("pre", s, n_tasks, wait_time_pre);
    ssts::utils::check_tasks("post", s, n_tasks, wait_time_post);
}

void t_every()
{
    /// task_scheduler::every() APIs
    ssts::utils::log_test("task_scheduler::every() APIs");

    const unsigned n_tasks { 3 };
    ssts::task_scheduler s(n_tasks);
    
    ssts::utils::timer t;
    
    const auto wait_time_pre = 0s;
    const auto wait_time_post = 3s;

    // TaskFunction only (no task_id, no parameters)
    s.every(1s, []{std::cout << "Hello A!" << std::endl;});
    ssts::utils::check_tasks("pre", s, n_tasks, wait_time_pre);
    ssts::utils::check_tasks("post", s, n_tasks, wait_time_post);

    // TaskFunction with parameters (no task_id)
    s.every(1s, [](auto p){std::cout << "Hello B! " << p << std::endl;}, "(with parameters)");
    ssts::utils::check_tasks("pre", s, n_tasks, wait_time_pre);
    ssts::utils::check_tasks("post", s, n_tasks, wait_time_post);

    // TaskFunction with parameters and task_id
    s.every("task_id_3"s, 1s, [](auto p){std::cout << "Hello C! " << p << std::endl;}, "(with parameters)");
    ssts::utils::check_tasks("pre", s, n_tasks, wait_time_pre);
    ssts::utils::check_tasks("post", s, n_tasks, wait_time_post);
}

void t_disable()
{
    /// Disable all
    ssts::utils::log_test("Disable all");

    const unsigned n_tasks { 3 };
    ssts::task_scheduler s(n_tasks);
    
    ssts::utils::timer t;
    
    const auto wait_time_pre = 1s;
    const auto wait_time_post = 3s;

    // Add "task_id_1"
    s.in("task_id_1"s, 2s, []{std::cout << "Hello A!" << std::endl;});
    s.in("task_id_2"s, 2s, []{std::cout << "Hello B!" << std::endl;});
    s.in("task_id_3"s, 2s, []{std::cout << "Hello C!" << std::endl;});

    // Disable tasks before they can run
    s.set_enabled("task_id_1"s, false);
    s.set_enabled("task_id_2"s, false);
    s.set_enabled("task_id_3"s, false);

    // Every task is scheduled, but none is enabled
    ssts::utils::check_tasks("pre - scheduled but disabled", s, n_tasks, wait_time_pre);
    ssts::utils::check_tasks("post - disabled and removed", s, n_tasks, wait_time_post);
}

void t_remove()
{
    /// Remove all
    ssts::utils::log_test("Remove all");

    const unsigned n_tasks { 3 };
    ssts::task_scheduler s(n_tasks);
    
    ssts::utils::timer t;
    
    const auto wait_time_pre = 1s;
    const auto wait_time_post = 3s;

    // Add "task_id_1"
    s.in("task_id_1"s, 2s, []{std::cout << "Hello A!" << std::endl;});
    s.in("task_id_2"s, 2s, []{std::cout << "Hello B!" << std::endl;});
    s.in("task_id_3"s, 2s, []{std::cout << "Hello C!" << std::endl;});

    // Remove tasks before they can run
    s.remove_task("task_id_1"s);
    s.remove_task("task_id_2"s);
    s.remove_task("task_id_3"s);

    // Every task is removed
    ssts::utils::check_tasks("pre - scheduled and enabled", s, n_tasks, wait_time_pre);
    ssts::utils::check_tasks("post - removed and disabled", s, n_tasks, wait_time_post);
}

void t_enable_disable_add_remove()
{
    /// Enable/Disable + Remove
    ssts::utils::log_test("Enable/Disable/Add/Remove");

    const unsigned n_tasks { 3 };
    ssts::task_scheduler s(n_tasks);
    
    ssts::utils::timer t;
    
    const auto wait_time = 5s;

    // Add "task_id_1"
    s.every("task_id_1"s, 1s, [](auto p){std::cout << "Hello A! " << p << std::endl;}, "(with task id 1)");
    ssts::utils::check_tasks("check task_id_1 added", s, n_tasks, wait_time);

    // Add "task_id_2"
    s.every("task_id_2"s, 1s, [](auto p){std::cout << "Hello B! " << p << std::endl;}, "(with task id 2)");
    ssts::utils::check_tasks("check task_id_2 added", s, n_tasks, wait_time);

    // Disable "task_id_1"
    s.set_enabled("task_id_1"s, false);
    // Add "task_id_3"
    s.every("task_id_3"s, 1s, [](auto p){std::cout << "Hello C! " << p << std::endl;}, "(with task id 3)");
    ssts::utils::check_tasks("check task_id_1 disabled, task_id_3 added", s, n_tasks, wait_time);

    // Enable "task_id_1"
    s.set_enabled("task_id_1"s, true);
    ssts::utils::check_tasks("check task_id_1 enabled", s, n_tasks, wait_time);

    // Remove "task_id_2"
    s.remove_task("task_id_2"s);
    ssts::utils::check_tasks("check task_id_2 removed", s, n_tasks, wait_time);
}

int main()
{
    ssts::utils::log(ssts::version());

    t_simple();
    t_return_value();
    
    t_in();
    t_at();
    t_every();

    t_task_pool_size_too_small();  // Fixed deadlock on scheduler shutdown.
    t_task_pool_size_ok(); // Fixed deadlock on scheduler shutdown.

    t_disable();
    t_remove();
    t_enable_disable_add_remove();

    /* TODO!

    t_re_enable();
    t_re_add();
    t_force_stop();

    */
    
    ssts::utils::log("Task Scheduler finished");
    return 0;
}