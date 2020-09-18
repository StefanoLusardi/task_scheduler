#pragma once

#include <map>
#include <chrono>
#include <functional>

#include "task.hpp"
#include "task_pool.hpp"

using namespace std::chrono_literals;

namespace ts
{

using clock = std::chrono::steady_clock;

std::string version() { return "Task Scheduler v1.0.0"; }

class task_scheduler
{
public:
    task_scheduler(const unsigned int num_threads = std::thread::hardware_concurrency()) 
    : _tp{ts::task_pool(num_threads)}
    , _is_running{ true }
    {
        _update_task_thread = std::thread([this] {
            while (_is_running)
            {
                std::unique_lock lock(_update_tasks_mtx);

                if (_tasks.empty())
                {
                    _update_tasks_cv.wait(lock, [this] 
                    { 
                        return !_tasks.empty() || !_is_running; 
                    });
                }
                else
                {
                    _update_tasks_cv.wait_until(lock, _tasks.begin()->first, [this] 
                    {
                        return ts::clock::now() >= _tasks.begin()->first || !_is_running; 
                    });
                }

                lock.unlock();
                update_tasks();
            }
        });
    }

    ~task_scheduler()
    { 
        if(_is_running)
            stop(); 
    }

    // std::future<void> stop();
    void stop(bool wait_for_running_tasks = true) 
    { 
        _is_running = false;
        _update_tasks_cv.notify_one();

        if(_update_task_thread.joinable());
            _update_task_thread.join();
    }
    
    // template <typename T>
    // std::future<T> every() { }
    // bool is_scheduled(task_id id) { }
    // bool remove_task(task_id id) { }

    template <typename TaskFunction, typename... Args>
    auto at(ts::clock::time_point&& timepoint, TaskFunction&& func, Args&&...args) 
        -> std::future<std::invoke_result_t<TaskFunction, Args...>>
    {
        auto task = [t=std::forward<TaskFunction>(func), params=std::make_tuple(std::forward<Args>(args)...)]
        {
            return std::apply(t, params);
        };

        using ReturnType = std::invoke_result_t<TaskFunction, Args...>;
        auto task_wrapper = std::packaged_task<ReturnType()>(task);
        std::future<ReturnType> future = task_wrapper.get_future();
        
        add_task(std::move(timepoint), std::move(task_wrapper));
        return future;
    }

    template <typename TaskFunction, typename... Args>
    auto in(ts::clock::duration&& duration, TaskFunction&& func, Args&&...args) 
        -> std::future<std::invoke_result_t<TaskFunction, Args...>>
    {
        return at(
            std::forward<ts::clock::time_point>(duration + ts::clock::now()), 
            std::forward<TaskFunction>(func),
            std::forward<Args>(args)...
        );
    }  

private:
    void add_task(ts::clock::time_point&& timepoint, ts::task&& st)
    {
        {
            std::scoped_lock lock(_update_tasks_mtx);
            _tasks.emplace(std::move(timepoint), std::move(st));
        }
        _update_tasks_cv.notify_one();
    }

    void update_tasks()
    {
        std::lock_guard lock(_update_tasks_mtx);
        auto tasks_to_run = _tasks.upper_bound(ts::clock::now());
        // decltype(_tasks) tasks_to_add;
        
        for (auto it = _tasks.begin(); it != tasks_to_run; ++it)
        {
            auto t = std::make_shared<ts::task>(std::move(it->second));
            _tp.run([t=std::move(t)]() { t->invoke(); });

            // if (!t->_is_one_shot)
            // tasks_to_add.insert(t.get_start_time(), t);
        }

        _tasks.erase(_tasks.begin(), tasks_to_run);
        // _tasks.merge(tasks_to_add);
    }

    ts::task_pool _tp;
    std::atomic_bool _is_running;
    std::multimap<ts::clock::time_point, ts::task> _tasks;
    std::condition_variable _update_tasks_cv;
    std::mutex _update_tasks_mtx;
    std::thread _update_task_thread;   
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
