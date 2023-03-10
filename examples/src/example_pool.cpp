#include <iostream>
#include <ssts/task_scheduler.hpp>

int main()
{
    std::cout << ssts::version() << "\n\n";

    std::vector<std::future<int>> futures;
    ssts::task_pool tp(8);
    int sum = 0;

    const int num_task = 1'000;

    std::mutex cout_mtx;
    for (int i = 0; i < num_task; ++i)
    {
        auto f = tp.run([i, &sum, &cout_mtx]() mutable 
        {
            std::scoped_lock lock(cout_mtx);
            std::cout 
                << "Thread ID: " << std::this_thread::get_id()
                << " --- Task ID: " << i 
                << " --- Current sum: " << sum  
                << std::endl;
            return ++sum;
        });

        futures.push_back(std::move(f));
    }

    for (int i = 0; i < num_task; ++i)
        futures[i].wait();

    std::cout << "\n\n" << " --- Results ready --- " << "\n\n" << std::endl;

    std::cout << "\n\n" << "Total sum: " << sum << "\n\n" << std::endl;

    return 0;
}
