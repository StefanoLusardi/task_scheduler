#pragma once

#include <map>
#include <chrono>
#include <functional>

#include "task_pool.hpp"
#include "schedulable_task.hpp"

using namespace std::chrono_literals;

namespace ts
{

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

        ~task_scheduler() { stop(); }

        // std::future<void> stop();
        void stop(bool wait_for_running_tasks = true) { _is_running=false; _update_tasks_cv.notify_one(); }
      
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
            // auto task_wrapper = std::make_shared<std::packaged_task<ReturnType()>>(task);
            std::future<ReturnType> future = task_wrapper.get_future();

            // auto st = ts::schedulable_task(std::move(task));
            // auto st = std::make_shared<ts::schedulable_task>(std::move(task));

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
                std::forward<Args>(args)...)
            );
        }  

    private:
        void add_task(ts::clock::time_point&& timepoint, ts::schedulable_task&& st)
        // void add_task(ts::clock::time_point&& timepoint, const std::shared_ptr<ts::schedulable_task>& st)
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
            
            for (auto it = _tasks.begin(); it != tasks_to_run; it++)
            {
                auto t = std::make_shared<ts::schedulable_task>(std::move(it->second));
                _tp.run([t=std::move(t)]()
                {
                    std::cout << "\nRunning on thread: " << std::this_thread::get_id() << std:: endl;
                    t->invoke();
                });

                // if (!task._is_one_shot)
                // tasks_to_add.insert(task.get_start_time(), task);
            }

            _tasks.erase(_tasks.begin(), tasks_to_run);
            // _tasks.merge(tasks_to_add);
        }
   
        std::atomic_bool _is_running;

        std::multimap<ts::clock::time_point, ts::schedulable_task> _tasks;
        // std::multimap<ts::clock::time_point, std::shared_ptr<ts::schedulable_task>> _tasks;
        ts::task_pool _tp;

        std::condition_variable _update_tasks_cv;
        std::mutex _update_tasks_mtx;

        std::thread _update_task_thread;   
    };

} // namespace ts

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