/*! 
 * \file task_scheduler.hpp 
 * \author Stefano Lusardi
 */

#pragma once

#include <chrono>
#include <map>
#include <unordered_set>
#include <optional>
#include <string>
#include <functional>

#include "task.hpp"
#include "task_pool.hpp"


using namespace std::chrono_literals;
using namespace std::string_literals;

namespace ssts
{

/*! \typedef ssts::clock Alias for std::chrono::steady_clock.
 */
using clock = std::chrono::steady_clock;

/*! ssts library version.  
 *  \return std::string current ssts version.  
*/ 
inline std::string version() { return "Task Scheduler v1.0.0"; }

/*! \class task_scheduler
 *  \brief Task Scheduler that can launch tasks on based several time-based policies.
 *
 *  This class is used to manage a queue of tasks using a fixed number of threads.  
 *  The actual task execution is delgated to an internal ssts::task_pool object.
 */
class task_scheduler
{
private:
    class schedulable_task
    {
    public:
        template<typename FunctionType>
        explicit schedulable_task(FunctionType&& f) 
        : _task{ssts::task(std::forward<FunctionType>(f))}
        , _is_enabled{true}
        { }

        template<typename FunctionType>
        explicit schedulable_task(FunctionType&& f, size_t hash) 
        : _task{ssts::task(std::forward<FunctionType>(f))}
        , _is_enabled{true}
        , _hash{hash}
        { }

        template<typename FunctionType>
        explicit schedulable_task(FunctionType&& f, ssts::clock::duration interval) 
        : _task{ssts::task(std::forward<FunctionType>(f))}
        , _is_enabled{true}
        , _interval{interval}
        { }

        template<typename FunctionType>
        explicit schedulable_task(FunctionType&& f, size_t hash, ssts::clock::duration interval) 
        : _task{ssts::task(std::forward<FunctionType>(f))}
        , _is_enabled{true}
        , _hash{hash}
        , _interval{interval}
        { }

        ~schedulable_task() { }

        schedulable_task(schedulable_task&) = delete;
        schedulable_task(const schedulable_task&) = delete;
        schedulable_task& operator=(const schedulable_task&) = delete;
        
        schedulable_task(schedulable_task&& other) noexcept 
        : _task{std::move(other._task)}
        , _is_enabled{std::move(other._is_enabled)}
        , _interval{std::move(other._interval)}
        , _hash{std::move(other._hash)}
        { }

        void invoke() { _task(); }

        ssts::task _task;
        bool _is_enabled;
        std::optional<ssts::clock::duration> _interval;
        std::optional<size_t> _hash;
    };

public:
    /*!
     * \brief Constructor.
     * \param num_threads Number of threads that will be used in the underlying ssts::task_pool.
     * 
     * Creates a ssts::task_scheduler instance. 
     * The number of threads to be used by the ssts::task_pool defaults to the number of threads supported by the platform.
     */
    explicit task_scheduler(const unsigned int num_threads = std::thread::hardware_concurrency())
    : _tp{num_threads}
    , _is_running{true}
    { 
        _scheduler_thread = std::thread([this]
        {
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

    /*!
     * \brief Destructor.
     * 
     * Destructs this. If the task_scheduler is running its tasks are stopped first.
     */
    ~task_scheduler()
    {
        if (_is_running)
            stop();
    }

    /*!
     * \brief Stop all running tasks.
     * 
     * This function stops the task_scheduler execution and stops all the running tasks.
     */
    void stop()
    {        
        _is_running = false;
        _update_tasks_cv.notify_all();

        if (_scheduler_thread.joinable())
            _scheduler_thread.join();
    }

    /*!
     * \brief Check if a task is scheduled.
     * \param task_id task_id to check.
     * \return bool indicating if the task is currently scheduled.
     * 
     * If a task has been started without a task_id it is not possible to query its status.
     * In case a task_id is not found this function return false.
     * If a task is no longer scheduled it must be added using one of the following APIs:
     * ssts::task_scheduler::in, ssts::task_scheduler::at, ssts::task_scheduler::every.
     */
    bool is_scheduled(const std::string& task_id)
    { 
        std::scoped_lock lock(_update_tasks_mtx);

        return get_task_iterator(task_id) != _tasks.end();
    }

    /*!
     * \brief Check if a task is enabled.
     * \param task_id task_id to check.
     * \return bool indicating if the task is currently enabled.
     * 
     * If a task has been started without a task_id it is not possible to query its status.
     * In case a task_id is not found this function return false.
     * By default new tasks are enabled.
     * A task can be enabled or disabled by calling ssts::task_scheduler::set_enabled.
     */
    bool is_enabled(const std::string& task_id)
    { 
        std::scoped_lock lock(_update_tasks_mtx);

        if (auto task = get_task_iterator(task_id); task != _tasks.end())
            return task->second._is_enabled;
        
        return false;
    }
    
    /*!
     * \brief Enable or disable task.
     * \param task_id task_id to enable or disable.
     * \param is_enabled true enables, false disables the given task_id.
     * \return bool indicating if the task is currently enabled.
     * 
     * If a task has been started without a task_id it is not possible to update its status.
     * In case a task_id is not found this function return false.
     * It is possible to check if a task is enabled or disabled by calling ssts::task_scheduler::is_enabled.
     */
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

    /*!
     * \brief Remove a task.
     * \param task_id task_id to remove.
     * \return bool indicating if the task has been properly removed.
     * 
     * If a task has been started without a task_id it is not possible to remove it.
     * In case a task_id is not found this function return false.
     * It is possible to check if a task is scheduled by calling ssts::task_scheduler::is_scheduled.
     */
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
    std::thread _scheduler_thread;
    std::multimap<ssts::clock::time_point, schedulable_task> _tasks;
    std::unordered_set<size_t> _tasks_to_delete;
    std::condition_variable _update_tasks_cv;
    std::mutex _update_tasks_mtx;
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
