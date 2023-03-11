/*! 
 * \file task_pool.hpp 
 * \author Stefano Lusardi
 */

#pragma once

#include <atomic>
#include <optional>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <future>
#include <condition_variable>
#include <unordered_set>

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
    , _is_duplicate_allowed{ true }
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
    auto run(FunctionType&& f, const std::optional<size_t>& task_hash = std::nullopt)
    {
        using result_type = std::invoke_result_t<std::decay_t<FunctionType>>;
        std::packaged_task<result_type()> task(std::forward<FunctionType>(f));
        std::future<result_type> future = task.get_future();

        std::unique_lock lock(_task_mtx);

        if(!_is_duplicate_allowed && is_already_running(task_hash))
            return future;

        auto hash = task_hash.value_or(0);
        _task_queue.emplace(hash, std::move(task));
        lock.unlock();
        _task_cv.notify_one();

        return future;
    }

    void set_duplicate_allowed(bool is_allowed) 
    {
        _is_duplicate_allowed = is_allowed; 
    }

private:
    std::atomic_bool _is_running;
    std::atomic_bool _is_duplicate_allowed;
    std::vector<std::thread> _threads;
    std::queue<std::pair<size_t, ssts::task>> _task_queue;
    std::unordered_set<size_t> _active_hash_set;
    std::condition_variable _task_cv;
    std::mutex _task_mtx;
    std::mutex _hash_mtx;

    void worker_thread()
    {
        while (_is_running)
        {
            std::unique_lock lock(_task_mtx);
            _task_cv.wait(lock, [this] { return !_task_queue.empty() || !_is_running; });

            if (!_is_running)
                return;

            auto task = std::move(_task_queue.front().second);
            auto hash = _task_queue.front().first;

            if(!_is_duplicate_allowed)
            {
                std::scoped_lock hash_lock(_hash_mtx);
                _active_hash_set.insert(hash);
            }

            _task_queue.pop();

            lock.unlock();
            task();

            if(!_is_duplicate_allowed)
            {
                std::scoped_lock hash_lock(_hash_mtx);
                if (_active_hash_set.find(hash) != _active_hash_set.end())
                    _active_hash_set.erase(hash);
            }
        }
    }

    bool is_already_running(const std::optional<size_t>& opt_hash)
    {
        if (!opt_hash.has_value())
            return false;

        std::unique_lock lock(_hash_mtx);
        return _active_hash_set.find(opt_hash.value()) != _active_hash_set.end();
    }
};

}
