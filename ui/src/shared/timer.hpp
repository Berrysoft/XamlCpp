#ifndef XMAL_UI_SHARED_TIMER_HPP
#define XMAL_UI_SHARED_TIMER_HPP

#include <atomic>
#include <xaml/event.h>
#include <xaml/ui/timer.h>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#endif // XAML_UI_WINDOWS

struct xaml_timer_impl : xaml_implement<xaml_timer_impl, xaml_timer, xaml_object>
{
    XAML_PROP_IMPL(interval, std::int32_t, std::int32_t*, std::int32_t)
    XAML_PROP_IMPL_BASE(is_enabled, std::atomic_bool, bool*)
    XAML_EVENT_IMPL(tick)

    xaml_result XAML_CALL start() noexcept override;
    xaml_result XAML_CALL stop() noexcept override;

#ifdef XAML_UI_WINDOWS
    UINT_PTR m_id{};
#endif // XAML_UI_WINDOWS

    xaml_timer_impl() noexcept
    {
    }

    xaml_timer_impl(std::int32_t interval) noexcept : m_interval(interval) {}
};

#endif // !XMAL_UI_SHARED_TIMER_HPP
