#pragma once

#include <iomanip>
#include <iostream>
#include <spdlog/spdlog.h>

#define ENABLE_PRINT_TIMING

using namespace std::chrono_literals;
using namespace std::string_literals;

namespace ssts::utils
{

void log(const std::string& msg)
{
    std::cout << "\n[ :: " << msg << " :: ]\n" <<std::endl;
}

void log_test(const std::string& msg, unsigned line_width = 40)
{
    std::cout << "\n"
        << "[ " << std::setfill(':') << ':' << std::setw(line_width) << " ]"
        << "\n[ :: " << std::setfill(' ') << std::setw(line_width-7) << msg << " :: ]\n"
        << "[ " << std::setfill(':') << ':' << std::setw(line_width) << " ]"
        << std::endl;
}

void check_tasks(const std::string& msg, ssts::task_scheduler& s, unsigned num_tasks, std::chrono::seconds wait_time = 5s)
{
    std::this_thread::sleep_for(wait_time);
    log(msg);
    
    for(unsigned i = 1; i <= num_tasks; ++i)
    {
        const auto task_id = "task_id_"s + std::to_string(i);
        std::cout << std::boolalpha << task_id << " enabled: " << s.is_enabled(task_id) << std::endl;
        std::cout << std::boolalpha << task_id << " scheduled: " << s.is_scheduled(task_id) << std::endl;
    }
    std::cout << "\n" << std::endl;
}

void check_sanitizer()
{
    std::mutex mutex;
    int a = 3;
    const size_t size = 1000 * 1000;
    std::atomic<int> b(1);
    
    std::thread t1([&]
    {
        for (size_t counter = 0; counter < size; counter++)
        {
            ++b;
            // std::unique_lock<std::mutex> lock(mutex);
            ++a;
        }
    });
    
    std::thread t2([&]
    {
        for (size_t counter = 0; counter < size; counter++)
        {
            --b;
            // std::unique_lock<std::mutex> lock(mutex);
            --a;
        }
    });

    t1.join();
    t2.join();
}

class timer
{
public:
    explicit timer() { start(); }
    ~timer() { stop(); }

protected:
    void start()
    {
        #if !defined(ENABLE_PRINT_TIMING)
            return;
        #endif

        is_running = true;
        timer_thread = std::thread([this]
        {
            unsigned i = 0; 
            while(is_running) 
            {
                spdlog::info("[{}]", i++);
                std::this_thread::sleep_for(1s);
            }
        });
    }

    void stop()
    {
        #if !defined(ENABLE_PRINT_TIMING)
            return;
        #endif

        if (!is_running)
            return;

        is_running.store(false);        
        if (timer_thread.joinable())
            timer_thread.join();
    }

private:
    std::atomic_bool is_running;
    std::thread timer_thread;
};

}

/*
// Debug Helpers:

template<class TupType, size_t... I>
void print_tuple(const TupType& _tup, std::index_sequence<I...>)
{
    std::cout << "(";
    (..., (std::cout << (I == 0? "" : ", ") << std::get<I>(_tup)));
    std::cout << ")\n";
}

template<class... T>
void print_tuple(const std::tuple<T...>& _tup)
{
    print_tuple(_tup, std::make_index_sequence<sizeof...(T)>());
}
*/