/*! 
 * \file task.hpp 
 * \author Stefano Lusardi
 */

#pragma once

#include <memory>
#include <algorithm>
#include <type_traits>


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
private:
    struct task_base
    {
        virtual ~task_base() { }
        virtual void invoke() = 0;
    };

    template<typename FunctionType>
    struct task_impl : task_base
    {
        explicit task_impl(FunctionType&& f) 
        : _func{ std::move(f) } 
        { 
            static_assert(std::is_invocable_v<decltype(f)>); 
        }
        void invoke() override { _func(); }
        FunctionType _func;
    };

public:
    /*!
     * \brief Default constructor.
     * \param f Callable parameterless object wrapped within this task instance.
     * 
     * Creates a task instance with the given callable object.
     * The callable object can be e.g. a lambda function, a functor, a free function or a class method bound to an object.
     */
    template<typename FunctionType>
    explicit task(FunctionType&& f) 
    : _impl{ std::make_unique<task_impl<FunctionType>>(std::move(f)) } 
    { }

    task(task&) = delete;
    task(const task&) = delete;
    task& operator=(const task&) = delete;
    
    /*!
     * \brief Move constructor.
     * \param other task object.
     * 
     * Move constructs a task instance to this.
     */
    task(task&& other) noexcept 
    : _impl{ std::move(other._impl) } 
    { }
    
    /*!
     * \brief operator().
     * 
     * Invokes a task.
     */
    void operator()() { _impl->invoke(); }
    
    /*!
     * \brief invoke().
     * 
     * Invokes a task.
     * Explicit overload of operator().
     */
    void invoke() { _impl->invoke(); }

private:
    std::unique_ptr<task_base> _impl;
};

}

