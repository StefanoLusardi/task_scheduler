#include <ssts/task_scheduler.hpp>

#include "utils/utils.hpp"

class Foo 
{
public:
    explicit Foo(int i) : x{i} { }
    void update() { ++x; }
    int get() { return x; }
private:
    int x;
};

void t_bind_lambda() 
{
    ssts::utils::log_test("Lambda");
    ssts::task_scheduler s(2);
    ssts::utils::timer t;

    Foo f1(0);
    s.every("task_id_1"s, 1s, [&f1] { f1.update(); });
    ssts::utils::log("bind lambda");
    std::this_thread::sleep_for(5s);
    std::cout << "f1: " << f1.get() << std::endl;
    s.remove_task("task_id_1"s);
}

void t_bind_class_method() 
{
    ssts::utils::log_test("Class Method");
    ssts::task_scheduler s(2);
    ssts::utils::timer t;

    Foo f2(0);
    s.every("task_id_2"s, 1s, std::bind(&Foo::update, &f2));
    ssts::utils::log("bind class method");
    std::this_thread::sleep_for(5s);
    std::cout << "f2: " << f2.get() << std::endl;
    s.remove_task("task_id_2"s);
}

int main() 
{
    ssts::utils::log(ssts::version());

    t_bind_lambda();
    t_bind_class_method();

    ssts::utils::log("Task Scheduler finished");
    return 0;
}
