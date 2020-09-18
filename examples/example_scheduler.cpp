#include <iostream>
#include <task_scheduler.hpp>

using namespace std::chrono_literals;
using namespace std::string_literals;

int main()
{
    std::cout << ts::version() << "\n\n";

    ts::task_scheduler s(2);

    s.in(10s, []{std::cout << "The last one!" << std::endl;});

    s.at(ts::clock::now() + 1s, []{std::cout << "Hello!" << std::endl;});

    s.in(2s, 
    [](auto str1, auto str2) { 
        std::cout << "Multi Params: " << str2 << ", " << str1 << std::endl; 
    }, 
    42, 
    43);
    
    s.in(3s, 
    [](auto str){ 
        std::cout << "The number: " << str << std::endl; 
        return "HEY!"s; 
    }, 
    42);

    std::future f = s.in(1s, 
    []{ 
        std::cout << "Wait for my result" << std::endl;
        std::this_thread::sleep_for(1s);
        return "This-is-the-result"s; 
    });

    std::cout << "\nWaiting task result" << std::endl;
    std::cout << "Task result: " << f.get() << std::endl;

    s.stop();
    
    std::cout << "\n\nOK!" << std::endl;

    return 0;
}