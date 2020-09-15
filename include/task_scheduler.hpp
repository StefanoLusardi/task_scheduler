#pragma once

#include <map>
#include <chrono>
#include <functional>

#include "task_pool.hpp"

namespace ts
{

    using clock = std::chrono::steady_clock;

    std::string version() { return "Task Scheduler v1.0.0"; }

    /*
template<typename FunctionType>
class schedulable_task
{
    	
    schedulable_task(schedulable_task&) = delete;
    schedulable_task(const schedulable_task&) = delete;
	schedulable_task& operator=(const schedulable_task&) = delete;
	
    schedulable_task(schedulable_task&& other) noexcept 
    : _func{ std::move(other._func) }
    , _is_one_shot{other._is_one_shot}
    , _start_time{other._start_time}
    { }

	schedulable_task& operator=(schedulable_task&& other) noexcept
	{
		_func = std::move(other._func);
        _is_one_shot = other._is_one_shot;
        _start_time = other._start_time;
		return *this;
	}

	void operator()() { _func(); }

    bool _is_one_shot;
    ts::clock::time_point _start_time;

private:
	FunctionType _func;
};
*/

    class schedulable_task
    {
    public:
        struct task_base
        {
            virtual void invoke() = 0;
            virtual ~task_base() {}
        };

        template <typename FunctionType>
        struct task_impl : task_base
        {
            FunctionType func;

            task_impl(FunctionType &&f) : func{std::move(f)}
            {
                static_assert(std::is_invocable_v<decltype(f)>);
            }

            void invoke()
            {
                if constexpr (std::is_invocable_v<decltype(func)>)
                    func();
            }
        };

    public:
        template <typename FunctionType>
        schedulable_task(FunctionType &&f) : _impl{std::make_unique<task_impl<FunctionType>>(std::move(f))} {}

        schedulable_task(schedulable_task &) = delete;
        schedulable_task(const schedulable_task &) = delete;
        schedulable_task &operator=(const schedulable_task &) = delete;

        schedulable_task(schedulable_task &&other) noexcept : _impl{std::move(other._impl)} {}
        schedulable_task &operator=(schedulable_task &&other) noexcept
        {
            _impl = std::move(other._impl);
            return *this;
        }

        void operator()() { _impl->invoke(); }

        bool _is_one_shot;
        ts::clock::time_point _start_time;

    private:
        std::unique_ptr<task_base> _impl;
    };

    class task_scheduler
    {
    public:
        task_scheduler(const unsigned int num_threads = std::thread::hardware_concurrency()) : _tp{ts::task_pool(num_threads)}
        {
            _update_task_thread = std::thread([this] {
                while (_is_running)
                {
                    std::unique_lock lock(_update_tasks_mtx);

                    if (_tasks.empty())
                        _update_tasks_cv.wait(lock, [this] { return !_tasks.empty(); });
                    else
                        _update_tasks_cv.wait_until(lock, _tasks.begin()->first, [this] { return ts::clock::now() >= _tasks.begin()->first; });

                    lock.unlock();
                    update_tasks();
                }
            });
        }

        ~task_scheduler()
        {
            // stop();
        }

        // std::future<void> stop(bool wait_for_running_tasks = true) { }

        template <typename TaskFunction>
        auto at(ts::clock::time_point &&timepoint, TaskFunction &&t) // -> std::future<std::invoke_result_t<std::decay_t<TaskFunction>>>
        {
            using ReturnType = std::invoke_result_t<std::decay_t<TaskFunction>>;

            auto schedulable_t = ts::schedulable_task(std::forward<TaskFunction &&>(t));
            add_task(std::move(timepoint), std::move(schedulable_t));

            // auto task = std::packaged_task<ReturnType()>(std::forward<TaskFunction>(t));
            // auto future = task.get_future();

            // return future;
        }

        // template <typename T>
        // std::future<T> at() { }

        // template <typename T>
        // std::future<T> every() { }

        // bool is_scheduled(task_id id) { }

        // bool remove_task(task_id id) { }

    private:
        std::atomic_bool _is_running;

        std::multimap<ts::clock::time_point, ts::schedulable_task> _tasks;
        ts::task_pool _tp;

        std::condition_variable _update_tasks_cv;
        std::mutex _update_tasks_mtx;

        std::thread _update_task_thread;

        void add_task(ts::clock::time_point &&timepoint, ts::schedulable_task &&t)
        {
            {
                std::scoped_lock lock(_update_tasks_mtx);
                _tasks.emplace(timepoint, std::move(t));
            }

            _update_tasks_cv.notify_one();
        }

        void update_tasks()
        {
            std::lock_guard lock(_update_tasks_mtx);
            auto tasks_to_run = _tasks.upper_bound(ts::clock::now());

            for (auto it = _tasks.begin(); it != tasks_to_run; ++it)
            {
                auto task = std::move(it->second);
                _tp.run([&task]() 
                {
                    std::cout << "inner task" << std::endl;
                    task();
                });

                // if (!task._is_one_shot)
                // _tasks.insert(task.get_start_time(), task);
            }

            _tasks.erase(_tasks.begin(), tasks_to_run);
        }
    };

} // namespace ts