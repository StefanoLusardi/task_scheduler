#pragma once

#include <atomic>
#include <chrono>
#include <map>
#include <unordered_set>
#include <optional>
#include <string>
#include <mutex>
#include <thread>
#include <future>
#include <condition_variable>
#include <functional>

#include "task.hpp"
#include "task_pool.hpp"

using namespace std::chrono_literals;

namespace ssts
{

using clock = std::chrono::steady_clock;

std::string version() { return "Task Scheduler v1.0.0"; }

class task_scheduler
{
private:
    class schedulable_task
    {
    public:
        template<typename FunctionType>
        schedulable_task(FunctionType&& f) 
        : _task{ssts::task(std::forward<FunctionType>(f))}
        , _is_enabled{true}
        { }

        template<typename FunctionType>
        schedulable_task(FunctionType&& f, size_t hash) 
        : _task{ssts::task(std::forward<FunctionType>(f))}
        , _is_enabled{true}
        , _hash{hash}
        { }

        template<typename FunctionType>
        schedulable_task(FunctionType&& f, ssts::clock::duration interval) 
        : _task{ssts::task(std::forward<FunctionType>(f))}
        , _is_enabled{true}
        , _interval{interval}
        { }

        template<typename FunctionType>
        schedulable_task(FunctionType&& f, size_t hash, ssts::clock::duration interval) 
        : _task{ssts::task(std::forward<FunctionType>(f))}
        , _is_enabled{true}
        , _hash{hash}
        , _interval{interval}
        { }

        ~schedulable_task() { }

        // schedulable_task(schedulable_task&) = delete; // MSVC 2017 issue: C4521 multiple copy constructors specified
        schedulable_task(const schedulable_task&) = delete;
        schedulable_task& operator=(const schedulable_task&) = delete;
        
        schedulable_task(schedulable_task&& other) noexcept 
        : _task{std::move(other._task)}
        , _is_enabled{std::move(other._is_enabled)}
        , _interval{std::move(other._interval)}
        , _hash{std::move(other._hash)}
        { }

        schedulable_task& operator=(schedulable_task&& other) noexcept
        {
            _task = std::move(other._task);
            _is_enabled = std::move(other._is_enabled);
            _interval = std::move(other._interval);
            _hash = std::move(other._hash);
            return *this;
        }

    	void invoke() { _task(); }

        ssts::task _task;
        bool _is_enabled;
        std::optional<ssts::clock::duration> _interval;
        std::optional<size_t> _hash;
    };

public:
    task_scheduler(const unsigned int num_threads = std::thread::hardware_concurrency())
    : _tp{ssts::task_pool(num_threads)}, _is_running{true}
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
                        return ssts::clock::now() >= _tasks.begin()->first || !_is_running;
                    });
                }

                lock.unlock();
                update_tasks();
            }
        });
    }

    ~task_scheduler()
    {
        if (_is_running)
            stop();
    }

    void stop(bool wait_for_running_tasks = true)
    {        
        _is_running = false;
        _update_tasks_cv.notify_all();

        if (_update_task_thread.joinable())
            _update_task_thread.join();
    }

    bool is_scheduled(const std::string& task_id)
    { 
        std::scoped_lock lock(_update_tasks_mtx);

        return get_task_iterator(task_id) != _tasks.end();
    }

    bool is_enabled(const std::string& task_id)
    { 
        std::scoped_lock lock(_update_tasks_mtx);

        if (auto task = get_task_iterator(task_id); task != _tasks.end())
            return task->second._is_enabled;
        
        return false;
    }
    
    bool set_enabled(const std::string& task_id, bool is_enabled) 
    {
        std::scoped_lock lock(_update_tasks_mtx);

        if (auto task = get_task_iterator(task_id); task != _tasks.end())
        {
            task->second._is_enabled = is_enabled;
            return true;
        }
        
        return false;
    }

    bool remove_task(const std::string& task_id) 
    {
        std::scoped_lock lock(_update_tasks_mtx);

        if (auto task = get_task_iterator(task_id); task != _tasks.end() && task->second._hash.has_value())
        {
            _tasks_to_delete.insert(task->second._hash.value());
            return true;
        }
        
        return false;
    }

    template <typename TaskFunction>
    auto at(ssts::clock::time_point &&timepoint, TaskFunction &&func)
        -> std::future<std::invoke_result_t<TaskFunction>>
    {
        auto task = [t = std::forward<TaskFunction>(func)] 
        {
            return t();
        };

        using ReturnType = std::invoke_result_t<TaskFunction>;
        auto task_wrapper = std::packaged_task<ReturnType()>(task);
        std::future<ReturnType> future = task_wrapper.get_future();

        add_task(std::move(timepoint), schedulable_task(std::move(task_wrapper)));
        return future;
    }

    template <typename TaskFunction, typename... Args>
    auto at(ssts::clock::time_point &&timepoint, TaskFunction &&func, Args &&... args)
        -> std::future<std::invoke_result_t<TaskFunction, Args...>>
    {
        auto task = [t = std::forward<TaskFunction>(func), params = std::make_tuple(std::forward<Args>(args)...)] 
        {
            return std::apply(t, params);
        };

        using ReturnType = std::invoke_result_t<TaskFunction, Args...>;
        auto task_wrapper = std::packaged_task<ReturnType()>(task);
        std::future<ReturnType> future = task_wrapper.get_future();

        add_task(std::move(timepoint), schedulable_task(std::move(task_wrapper)));
        return future;
    }

    template <typename TaskFunction, typename... Args>
    auto at(std::string&& task_id, ssts::clock::time_point &&timepoint, TaskFunction &&func, Args &&... args)
        -> std::future<std::invoke_result_t<TaskFunction, Args...>>
    {
        auto task = [t = std::forward<TaskFunction>(func), params = std::make_tuple(std::forward<Args>(args)...)] 
        {
            return std::apply(t, params);
        };

        using ReturnType = std::invoke_result_t<TaskFunction, Args...>;
        auto task_wrapper = std::packaged_task<ReturnType()>(task);
        std::future<ReturnType> future = task_wrapper.get_future();

        add_task(std::move(timepoint), schedulable_task(std::move(task_wrapper), _hasher(task_id)));
        return future;
    }


    template <typename TaskFunction>
    auto in(ssts::clock::duration&& duration, TaskFunction &&func)
        -> std::future<std::invoke_result_t<TaskFunction>>
    {
        return at(
            std::forward<ssts::clock::time_point>(ssts::clock::now() + duration),
            std::forward<TaskFunction>(func));
    }

    template <typename TaskFunction, typename... Args>
    auto in(ssts::clock::duration&& duration, TaskFunction &&func, Args &&... args)
        -> std::future<std::invoke_result_t<TaskFunction, Args...>>
    {
        return at(
            std::forward<ssts::clock::time_point>(ssts::clock::now() + duration),
            std::forward<TaskFunction>(func),
            std::forward<Args>(args)...);
    }

    template <typename TaskFunction, typename... Args>
    auto in(std::string&& task_id, ssts::clock::duration&& duration, TaskFunction &&func, Args &&... args)
        -> std::future<std::invoke_result_t<TaskFunction, Args...>>
    {
        return at(
            std::forward<std::string>(task_id),
            std::forward<ssts::clock::time_point>(ssts::clock::now() + duration),
            std::forward<TaskFunction>(func),
            std::forward<Args>(args)...);
    }


    template <typename TaskFunction>
    auto every(ssts::clock::duration&& interval, TaskFunction &&func)
    {
        auto task = [t = std::forward<TaskFunction>(func)] 
        {
            return t(); 
        };
        add_task(std::move(ssts::clock::now() + interval), schedulable_task(std::move(task), interval));
    }

    template <typename TaskFunction, typename... Args>
    auto every(ssts::clock::duration&& interval, TaskFunction &&func, Args &&... args)
    {
        auto task = [t = std::forward<TaskFunction>(func), params = std::make_tuple(std::forward<Args>(args)...)] 
        {
            return std::apply(t, params);
        };
        add_task(std::move(ssts::clock::now() + interval), schedulable_task(std::move(task), interval));
    }

    template <typename TaskFunction, typename... Args>
    auto every(std::string&& task_id, ssts::clock::duration&& interval, TaskFunction &&func, Args &&... args)
    {
        auto task = [t = std::forward<TaskFunction>(func), params = std::make_tuple(std::forward<Args>(args)...)] 
        {
            return std::apply(t, params);
        };
        add_task(std::move(ssts::clock::now() + interval), schedulable_task(std::move(task), _hasher(task_id), interval));
    }

private:
    ssts::task_pool _tp;
    std::atomic_bool _is_running;
    std::multimap<ssts::clock::time_point, schedulable_task> _tasks;
    std::unordered_set<size_t> _tasks_to_delete;
    std::condition_variable _update_tasks_cv;
    std::mutex _update_tasks_mtx;
    std::thread _update_task_thread;
    std::hash<std::string> _hasher;

    void add_task(ssts::clock::time_point&& timepoint, schedulable_task&& st)
    {
        if (!_is_running)
            return;

        {
            std::scoped_lock lock(_update_tasks_mtx);
            _tasks.emplace(std::move(timepoint), std::move(st));
        }
        _update_tasks_cv.notify_one();
    }

    void update_tasks()
    {
        std::scoped_lock lock(_update_tasks_mtx);

        auto last_task_to_process = _tasks.upper_bound(ssts::clock::now());
        for (auto it = _tasks.begin(); it != last_task_to_process; it++)
        {
            // If a task has been marked to be deleted, just clean it up from the _tasks_to_delete set.
            // Do not schedule it within the task pool.
            // It will be erased after the for loop with all the other processed tasks.
            if(it->second._hash.has_value())
            {
                if(const auto task_id = it->second._hash.value(); _tasks_to_delete.find(task_id) != _tasks_to_delete.end())
                {
                    _tasks_to_delete.erase(task_id);
                    continue;
                }
            }

            _tp.run([t=std::make_shared<schedulable_task>(std::move(it->second)), this]
            {
                if (t->_is_enabled)
                    t->invoke(); 

                if (t->_interval.has_value())
                    add_task(ssts::clock::now() + t->_interval.value(), std::move(*t)); 
            });
        }

        _tasks.erase(_tasks.begin(), last_task_to_process);
    }

    auto get_task_iterator(const std::string& task_id) -> decltype(_tasks)::iterator
    {
        return std::find_if(_tasks.begin(), _tasks.end(), [hash = _hasher(task_id)](auto&& it)
        {
            if (it.second._hash.has_value())
            {
                return hash == it.second._hash.value();
            }
            return false;
        });
    }
};

}

