/*! 
 * \file task_pool.hpp 
 * \author Stefano Lusardi
 * \date 2020-09-29
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


/*! \addtogroup ssts
 *  @{
 */
namespace ssts
{

/*! \class task_pool
 *  \brief Task Pool that capable of running \ref ssts::task any callable object.
 *
 *  This class is general purpose thread pool that can launch task asyncronously.
 *  It is possible to get an asyncronous result of a task execution.
 */
class task_pool
{
//! \private
public:
	explicit task_pool(const unsigned int num_threads = std::thread::hardware_concurrency()) : _is_running{ true }
	{
		const auto thread_count = std::min(num_threads, std::thread::hardware_concurrency());
		try
		{
			for (unsigned i = 0; i < thread_count; ++i)
			{
				_threads.emplace_back([this] { worker_thread(); });
			}
		}
		catch (...)
		{
			_is_running = false;
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
		_is_running = false;
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
		_task_queue.push(std::move(task));
		lock.unlock();
		_task_cv.notify_one();

		return future;
	}

//! \private
private: 
	std::atomic_bool _is_running;
	std::vector<std::thread> _threads;
	std::queue<ssts::task> _task_queue;
	std::condition_variable _task_cv;
	std::mutex _task_mtx;

	void worker_thread()
	{
		while (true)
		{
			std::unique_lock lock(_task_mtx);
			_task_cv.wait(lock, [this] { return !_task_queue.empty() || !_is_running; });

			if (!_is_running) 
                break;

			// static_assert(!_task_queue.empty());
			auto task = std::move(_task_queue.front());
			_task_queue.pop();

			lock.unlock();
			task();
		}
	}
};

}
/*! @} */