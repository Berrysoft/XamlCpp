#ifndef XMAL_UI_SHARED_TIMER_HPP
#define XMAL_UI_SHARED_TIMER_HPP

#include <atomic>
#include <xaml/event.h>
#include <xaml/ui/timer.h>

#ifdef XAML_UI_WINDOWS
    #include <Windows.h>
#elif defined(XAML_UI_COCOA)
    #include <xaml/ui/cocoa/objc.h>
#elif defined(XAML_UI_GTK3)
    #include <gtk/gtk.h>
#elif defined(XAML_UI_QT5)
    #include <QTimer>
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
#elif defined(XAML_UI_COCOA)
    using native_timer_type = OBJC_OBJECT(NSTimer);
    using native_delegate_type = OBJC_OBJECT(XamlDelegate);

    native_timer_type m_handle{ OBJC_NIL };
    native_delegate_type m_delegate{ OBJC_NIL };

    void on_tick() noexcept;
#elif defined(XAML_UI_GTK3)
    static gboolean on_timeout(xaml_timer_impl* self) noexcept;
#elif defined(XAML_UI_QT5)
    QTimer m_handle{};
#endif // XAML_UI_WINDOWS

    xaml_timer_impl() noexcept
    {
    }

    xaml_timer_impl(std::int32_t interval) noexcept : m_interval(interval)
    {
    }

    xaml_result XAML_CALL init() noexcept;
};

#endif // !XMAL_UI_SHARED_TIMER_HPP
