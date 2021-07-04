#ifndef XAML_ASYNC_ACTION_H
#define XAML_ASYNC_ACTION_H

#include <xaml/async/info.h>
#include <xaml/delegate.h>
#include <xaml/meta/meta_macros.h>
#include <xaml/vector.h>

#ifdef __cplusplus
    #include <atomic>
    #include <coroutine>
    #include <xaml/ptr.hpp>
#endif // __cplusplus

XAML_CLASS(xaml_async_action, { 0xe31c7f48, 0x8af4, 0x46af, { 0xac, 0x54, 0x01, 0xa1, 0xc1, 0xea, 0xea, 0x1e } })

#ifndef xaml_delegate_2__xaml_async_action__xaml_async_status_defined
    #define xaml_delegate_2__xaml_async_action__xaml_async_status_defined
XAML_DELEGATE_2_TYPE(XAML_T_O(xaml_async_action), XAML_T_V(xaml_async_status))
#endif // !xaml_delegate_2__xaml_async_action__xaml_async_status_defined

#define XAML_ASYNC_ACTION_VTBL(type)               \
    XAML_VTBL_INHERIT(XAML_ASYNC_INFO_VTBL(type)); \
    XAML_PROP(completed, type, XAML_DELEGATE_2_NAME(xaml_async_action, xaml_async_status)**, XAML_DELEGATE_2_NAME(xaml_async_action, xaml_async_status)*)

XAML_DECL_INTERFACE_(xaml_async_action, xaml_async_info)
{
    XAML_DECL_VTBL(xaml_async_action, XAML_ASYNC_ACTION_VTBL);
};

#ifndef xaml_enumerator_1__xaml_async_action_defined
    #define xaml_enumerator_1__xaml_async_action_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_async_action))
#endif // !xaml_enumerator_1__xaml_async_action_defined

#ifndef xaml_vector_view_1__xaml_async_action_defined
    #define xaml_vector_view_1__xaml_async_action_defined
XAML_VECTOR_VIEW_1_TYPE(XAML_T_O(xaml_async_action))
#endif // !xaml_vector_view_1__xaml_async_action_defined

EXTERN_C XAML_ASYNC_API xaml_result XAML_CALL xaml_async_action_wait_all(XAML_VECTOR_VIEW_1_NAME(xaml_async_action) *) XAML_NOEXCEPT;

#ifdef __cplusplus
template <typename T, typename I>
struct xaml_async_promise_base : xaml_implement<T, I>
{
    std::exception_ptr m_exception{};
    std::atomic<xaml_async_status> m_status{};
    xaml_ptr<xaml_delegate<I, xaml_async_status>> m_handler{};

    std::coroutine_handle<T> get_handle() { return std::coroutine_handle<T>::from_promise(*static_cast<T*>(this)); }

    std::uint32_t XAML_CALL release() noexcept override
    {
        std::int32_t res = --this->m_ref_count;
        if (res == 0)
        {
            get_handle().destroy();
        }
        return res;
    }

    xaml_result XAML_CALL get_completed(xaml_delegate<I, xaml_async_status>** ptr) noexcept override
    {
        return m_handler.query(ptr);
    }

    xaml_result XAML_CALL set_completed(xaml_delegate<I, xaml_async_status>* h) noexcept override
    {
        m_handler = h;
        if (m_status != xaml_async_started)
        {
            return invoke_completed();
        }
        return XAML_S_OK;
    }

    xaml_result invoke_completed() noexcept
    {
        if (m_handler)
        {
            return m_handler->invoke(this, m_status);
        }
        return XAML_S_OK;
    }

    xaml_ptr<I> get_return_object() noexcept { return this; }
    std::suspend_always initial_suspend() const noexcept { return {}; }
    auto final_suspend() noexcept
    {
        struct awaitable
        {
            xaml_async_promise_base* m_promise;

            bool await_ready() const noexcept { return false; }
            void await_resume() const noexcept {}

            bool await_suspend(std::coroutine_handle<>) const noexcept
            {
                m_promise->invoke_completed();
                return m_promise->release() > 0;
            }
        };
        if (m_status == xaml_async_started) m_status = xaml_async_completed;
        return awaitable{ this };
    }

    void unhandled_exception() noexcept
    {
        m_exception = std::current_exception();
        m_status = xaml_async_error;
    }

    void rethrow_if_failed() const
    {
        if (m_status == xaml_async_error)
        {
            std::rethrow_exception(m_exception);
        }
    }

    xaml_result XAML_CALL get_error() noexcept override
    try
    {
        auto handle = get_handle();
        while (!handle.done()) handle.resume();
        rethrow_if_failed();
        return XAML_S_OK;
    }
    XAML_CATCH_RETURN()

    xaml_result XAML_CALL get_status(xaml_async_status* ptr) noexcept override
    {
        *ptr = m_status;
        return XAML_S_OK;
    }
};

auto operator co_await(xaml_ptr<xaml_async_action> action)
{
    struct awaitable
    {
        xaml_ptr<xaml_async_action> m_action;

        bool await_ready() const
        {
            xaml_async_status status;
            XAML_THROW_IF_FAILED(m_action->get_status(&status));
            return status != xaml_async_started;
        }

        void await_resume() const noexcept {}

        void await_suspend(std::coroutine_handle<> h) const
        {
            auto action = m_action;
            xaml_ptr<xaml_delegate<xaml_async_action, xaml_async_status>> handler;
            XAML_THROW_IF_FAILED(xaml_delegate_new(
                [h](xaml_ptr<xaml_async_action> const&, xaml_async_status) noexcept -> xaml_result {
                    try
                    {
                        h.resume();
                        return XAML_S_OK;
                    }
                    XAML_CATCH_RETURN()
                },
                &handler));
            XAML_THROW_IF_FAILED(action->set_completed(handler));
            XAML_THROW_IF_FAILED(action->get_error());
        }
    };
    return awaitable{ action };
}

namespace std
{
    template <typename... Args>
    struct coroutine_traits<xaml_ptr<xaml_async_action>, Args...>
    {
        struct promise_type final : xaml_async_promise_base<promise_type, xaml_async_action>
        {
            void return_void() const noexcept {}
        };
    };
} // namespace std
#endif // __cplusplus

#endif // !XAML_ASYNC_ACTION_H
