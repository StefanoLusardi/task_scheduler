#include <iostream>
#include <task_scheduler.hpp>

using namespace std::chrono_literals;

int main()
{
    std::cout << ts::version() << "\n\n";
    
    ts::task_scheduler s;

    s.at(ts::clock::now() + 5s, []{std::cout << "Hello!" << std::endl;});

    std::this_thread::sleep_for(7s);
    std::cout << "stop" << std::endl;

    return 0;
}