/*! 
 * \file task.hpp 
 * \author Stefano Lusardi
 * \date 2020-09-29
 */

#pragma once

#include <memory>
#include <algorithm>
#include <type_traits>

/*! \addtogroup ssts
 *  @{
 */
namespace ssts
{

/*! \class task
 *  \brief Move-only callable object.
 *
 *  This class represents a callable object. Can be initialized with any invocable type that supports operator().  
 *  Internally the class implements a type-erasure idiom to accept any callable signature without exposing it to the outside.
 */
class task
{
//! \private
private:
	struct task_base
	{
		virtual ~task_base() { }
		virtual void invoke() = 0;
	};

	template<typename FunctionType>
	struct task_impl : task_base
	{
		explicit task_impl(FunctionType&& f) : _func{ std::move(f) } { static_assert(std::is_invocable_v<decltype(f)>); }
		void invoke() override { _func(); }
		FunctionType _func;
	};

//! \public
public:
	/*!
	 * \brief Default constructor.
	 * \param f Callable parameterless object.
	 * 
	 * Creates a \ref ssts::task instance.
	 */
	template<typename FunctionType>
	task(FunctionType&& f) 
	: _impl{ std::make_unique<task_impl<FunctionType>>(std::move(f)) } 
	{ }

    task(task&) = delete;
    task(const task&) = delete;
	task& operator=(const task&) = delete;
	
	/*!
	 * \brief Move constructor.
	 * \param other \ref ssts::task object.
	 * 
	 * Move constructs a \ref ssts::task instance to this.
	 */
    task(task&& other) noexcept 
	: _impl{ std::move(other._impl) } 
	{ }
	
	/*!
	 * \brief Move assignement.
	 * \param other \ref ssts::task object.
	 * 
	 * Move assigns a \ref ssts::task instance to this.
	 */
	task& operator=(task&& other) noexcept
	{
		_impl = std::move(other._impl);
		return *this;
	}

	/*!
	 * \brief operator().
	 * 
	 * Invokes a \ref ssts::task.
	 */
	void operator()() { _impl->invoke(); }

//! \private
private:
	std::unique_ptr<task_base> _impl;
};

}
/*! @} */
