/*! 
 * \file task_pool.hpp 
 * \author Stefano Lusardi
 */

#pragma once

#include <atomic>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <future>
#include <condition_variable>

#include "task.hpp"

namespace ssts
{
/*! \class task_pool
 *  \brief Task Pool that can run any callable object.
 *
 *  This class is general purpose thread pool that can launch task asyncronously.
 *  It is possible to get an asyncronous result of a task execution.
 */
class task_pool
{
public:
    /*!
     * \brief Constructor.
     * \param num_threads Number of threads that will be used in the underlying ssts::task_pool.
     * 
     * Creates a ssts::task_pool instance with the given number of threads.
     */
    explicit task_pool(const unsigned int num_threads = std::thread::hardware_concurrency())
    : _is_running{ true }
    {
        const auto thread_count = std::clamp(num_threads, 1u, std::thread::hardware_concurrency());
        _threads.reserve(thread_count);
        try 
        {
            for (unsigned int i = 0; i < thread_count; ++i)
                _threads.emplace_back(&task_pool::worker_thread, this);
        } 
        catch (...) 
        {
            _is_running = false;
            _task_cv.notify_all();
            throw;
        }
    }

    task_pool(task_pool&) = delete;
    task_pool(const task_pool&) = delete;
    task_pool& operator=(const task_pool&) = delete;

    task_pool(task_pool&&) = delete;
    task_pool& operator=(task_pool&&) = delete;

    /*!
     * \brief Destructor.
     * 
     * Destructs this after all joinable threads are terminated.
     */
    ~task_pool()
    {
        if (_is_running)
            stop();
    }

    /*!
     * \brief Stop all threads.
     * 
     * Stop thread pool and join all joinable threads.
     */
    void stop()
    {
        {
            std::unique_lock<std::mutex> lock(_task_mtx);
            _is_running = false;
        }
        
        _task_cv.notify_all();

        for (auto&& t : _threads)
        {
            if (t.joinable())
                t.join();
        }
    }

    /*!
     * \brief Run a callable object asynchronously.
     * \tparam FunctionType Types of the callable object. 
     * \param f Callable object.
     * \return std::future task result
     * 
     * Enqueue a new task with the given callable object.
     * The enqueued task will run as soon as a thread is available.
     * Returns the result of the asynchronous computation.
     */
    template<typename FunctionType>
    auto run(FunctionType&& f)
    {
        using result_type = std::invoke_result_t<std::decay_t<FunctionType>>;

        std::packaged_task<result_type()> task(std::forward<FunctionType>(f));
        std::future<result_type> future = task.get_future();

        std::unique_lock lock(_task_mtx);
        _task_queue.emplace(std::move(task));
        lock.unlock();
        _task_cv.notify_one();

        return future;
    }

private:
    std::atomic_bool _is_running;
    std::vector<std::thread> _threads;
    std::queue<ssts::task> _task_queue;
    std::condition_variable _task_cv;
    std::mutex _task_mtx;

    void worker_thread()
    {
        while (_is_running)
        {
            std::unique_lock lock(_task_mtx);
            _task_cv.wait(lock, [this] { return !_task_queue.empty() || !_is_running; });

            if (!_is_running)
                return;

            auto task = std::move(_task_queue.front());
            _task_queue.pop();

            lock.unlock();
            task();
        }
    }
};

}
