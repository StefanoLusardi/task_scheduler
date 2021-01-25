#include <ssts/task_scheduler.hpp>

#include "utils/utils.hpp"

int main() 
{
    ssts::utils::log(ssts::version());

    // ssts::utils::check_sanitizer();

    ssts::utils::timer t;

    for(auto i = 0; i < 100'000; ++i)
    {
        ssts::utils::log(std::to_string(i));
        {
            ssts::task_scheduler tp(32);
            tp.stop();
        }
        std::this_thread::sleep_for(1ms);
    }
    
    ssts::utils::log_test("Scheduler shutdown finished succesfully!");

    return 0;
}
