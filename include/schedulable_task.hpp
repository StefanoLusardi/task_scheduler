#pragma once

#include <type_traits>
#include <functional>
#include <memory>

namespace ts
{

    using clock = std::chrono::steady_clock;

    class schedulable_task
    {
        struct task_base
        {
            virtual ~task_base() {}
            virtual void invoke() = 0;
        };

        template <typename FunctionType>
        struct task_impl : task_base
        {
            task_impl(FunctionType &&f) : _func{std::move(f)}{ static_assert(std::is_invocable_v<decltype(f)>); }
            void invoke() override { _func(); }
            FunctionType _func;
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
        void invoke() { _impl->invoke(); }

        bool _is_one_shot;
        ts::clock::time_point _start_time;

    private:
        std::unique_ptr<ts::schedulable_task::task_base> _impl;
    };

}
