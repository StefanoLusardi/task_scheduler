#pragma once

#include <memory>
#include <utility>
#include <type_traits>

namespace ts
{

class task
{
	struct task_base
	{
		virtual void call() = 0;
		virtual ~task_base() {}
	};

	template<typename FunctionType>
	struct task_impl : task_base
	{
		FunctionType func;

		task_impl(FunctionType&& f) : func{ std::move(f) }
		{
			static_assert(std::is_invocable_v<decltype(f)>);
		}

		void call()
		{
			if constexpr (std::is_invocable_v<decltype(func)>)
				func();
		}
	};

public:
	template<typename FunctionType>
	task(FunctionType&& f) : _impl{ std::make_unique<task_impl<FunctionType>>(std::move(f)) } { }
	
    task(task&) = delete;
    task(const task&) = delete;
	task& operator=(const task&) = delete;
	
    task(task&& other) noexcept : _impl{ std::move(other._impl) } { }
	task& operator=(task&& other) noexcept
	{
		_impl = std::move(other._impl);
		return *this;
	}

	void operator()() { _impl->call(); }

private:
	std::unique_ptr<task_base> _impl;
};

}
