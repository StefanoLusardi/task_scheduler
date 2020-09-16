#include <iostream>
#include <task_scheduler.hpp>

using namespace std::chrono_literals;
using namespace std::string_literals;

template <typename TaskFunction, typename... Args>
void wrap_invoke(TaskFunction&& t, Args&&...args)
{
    auto func = [t=std::move(t), args=std::make_tuple(std::forward<Args>(args) ...)]()mutable
    {
        return std::apply([t=std::move(t)](auto&& ... args)
        {
            return std::invoke(t, args...);
        }, std::move(args));
    };

    auto st = std::make_shared<ts::schedulable_task>(func);
    st->invoke();
}

template <typename TaskFunction, typename... Args>
void wrap(TaskFunction&& t, Args&&...args)
{
    auto func = [t=std::move(t), args=std::make_tuple(std::forward<Args>(args)...)]
    {
        return std::apply(t, args);
    };

    auto st = std::make_shared<ts::schedulable_task>(func);
    st->invoke();
}

int main()
{
    std::cout << ts::version() << "\n\n";

    // wrap([](auto str) { std::cout << str << std::endl; }, "str");
    // wrap([](auto str, auto i) { std::cout << str << i <<std::endl; }, "str", 42);
    
    ts::task_scheduler s(2);
    s.at(ts::clock::now() + 1s, []{std::cout << "Hello!" << std::endl;});
    s.at(ts::clock::now() + 2s, [](auto str1, auto str2) { std::cout << "Multi Params: " << str2 << ", " << str1 << std::endl; }, 42, 43);
    
    s.at(ts::clock::now() + 3s, 
    [](auto str){ 
        std::cout << "The number: " << str << std::endl; return "HEY!"s; 
    }, 
    42);

    std::future f = s.at(ts::clock::now() + 1s, 
    []{ 
        std::cout << "Wait for my result" << std::endl;
        std::this_thread::sleep_for(1s);
        return "This-is-the-result"s; 
    });

    std::cout << "\nWaiting task result" << std::endl;
    std::cout << "Task result: " << f.get() << std::endl;

    std::this_thread::sleep_for(5s);

    return 0;
}