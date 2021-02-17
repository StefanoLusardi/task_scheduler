#include <ssts/task_scheduler.hpp>

#include "utils/utils.hpp"

void main_loop() 
{
    ssts::utils::log(ssts::version());

    ssts::task_scheduler s(8);    
    s.set_duplicate_allowed(true);

    ssts::utils::timer t;

    /**
     * A: Loop Time > Processing Time
     * 
     * Task A is looping slower (1 second) than its processing time (400 millisseconds).
     * The execution rate is once per second even if the scheduler could satisfy smaller loop time (up to 400 milliseconds).
    */
    std::string idA = " ==:A:== ";
    std::atomic_int n_task_runA = 0;
    s.every(std::string(idA), 1s, [idA, &n_task_runA]
    {
        std::cout << idA << std::endl; 
        std::this_thread::sleep_for(400ms);
        ++n_task_runA;
    });

    /**
     * B: Loop Time < Processing Time
     * 
     * Task B is looping faster (10 milliseconds) than its processing time (5 seconds).
     * The execution is scheduled only when previous task is completed, so once every second.
    */
    std::string idB = " ==:B:== ";
    std::atomic_int n_task_runB = 0;
    s.every(std::string(idB), 1000ms, [idB, &n_task_runB]
    {
        std::cout << idB << std::endl; 
        std::this_thread::sleep_for(5s);
        ++n_task_runB;
    });

    std::string idC = " ==:C:== ";
    std::atomic_int n_task_runC = 0;
    s.every(std::string(idC), 250ms, [idC, &n_task_runC]
    {
        std::cout << idC << std::endl; 
        std::this_thread::sleep_for(5ms);
        ++n_task_runC;
    });

    std::string idD = " ==:D:== ";
    std::atomic_int n_task_runD = 0;
    s.every(std::string(idD), 250ms, [idD, &n_task_runD]
    {
        std::cout << idD << std::endl; 
        std::this_thread::sleep_for(400ms);
        ++n_task_runD;
    });

    // if(!s.set_enabled(idD, false))
    //     std::cout << "ERROR: set_enabled(idD, false) failed" << std::endl;
    
    // if(s.is_enabled(idD))
    //     std::cout << "ERROR: is_enabled(idD) failed" << std::endl;

    // std::this_thread::sleep_for(2s);
    // if(!s.set_enabled(idD, true))
    //     std::cout << "ERROR: set_enabled(idD, true) failed" << std::endl;

    // if(!s.is_enabled(idD))
    //     std::cout << "ERROR: is_enabled(idD) failed" << std::endl;

    // if(!s.is_scheduled(idA)) std::cout << "ERROR: is_scheduled(idA) failed" << std::endl;
    // if(!s.is_scheduled(idB)) std::cout << "ERROR: is_scheduled(idB) failed" << std::endl;
    // if(!s.is_scheduled(idC)) std::cout << "ERROR: is_scheduled(idC) failed" << std::endl;
    // if(!s.is_scheduled(idD)) std::cout << "ERROR: is_scheduled(idD) failed" << std::endl;
    // if(s.size() != 4) std::cout << "ERROR: size() != 4" << std::endl;

    std::this_thread::sleep_for(10s);

    std::cout << "task size: " << s.size() << std::endl;
    s.stop();

    std::cout << "\n\n" << std::endl;
    std::cout << idA << "tasks run: " << n_task_runA << std::endl;
    std::cout << idB << "tasks run: " << n_task_runB << std::endl;
    std::cout << idC << "tasks run: " << n_task_runC << std::endl;
    std::cout << idD << "tasks run: " << n_task_runD << std::endl;

    ssts::utils::log_test("Scheduler finished succesfully!");
}

int main() 
{
    // for(int i=0; i<1'000; ++i)
    main_loop();

    return 0;
}
