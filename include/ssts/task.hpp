#pragma once

#include <memory>
#include <algorithm>
#include <type_traits>

namespace ssts
{

class task
{
private:
	struct task_base
	{
		virtual ~task_base() { }
		virtual void invoke() = 0;
	};

	template<typename FunctionType>
	struct task_impl : task_base
	{
		task_impl(FunctionType&& f) : _func{ std::move(f) } { static_assert(std::is_invocable_v<decltype(f)>); }
		void invoke() override { _func(); }
		FunctionType _func;
	};

public:
	template<typename FunctionType>
	task(FunctionType&& f) 
	: _impl{ std::make_unique<task_impl<FunctionType>>(std::move(f)) } { }

    // task(task&) = delete; // MSVC 2017 issue: C4521 multiple copy constructors specified
    task(const task&) = delete;
	task& operator=(const task&) = delete;
	
    task(task&& other) noexcept : _impl{ std::move(other._impl) } { }
	task& operator=(task&& other) noexcept
	{
		_impl = std::move(other._impl);
		return *this;
	}

	void operator()() { _impl->invoke(); }

private:
	std::unique_ptr<task_base> _impl;
};

}
