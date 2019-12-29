#ifndef XAML_UI_TIMER_HPP
#define XAML_UI_TIMER_HPP

#include <atomic>
#include <chrono>
#include <memory>
#include <xaml/meta/meta_macro.hpp>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#elif defined(XAML_UI_GTK3)
#include <gtk/gtk.h>
#elif defined(XAML_UI_COCOA)
#include <xaml/ui/objc.hpp>
#if __OBJC__
#import <Cocoa/Cocoa.h>
#endif // __OBJC__
#endif

namespace xaml
{
    class timer : std::enable_shared_from_this<timer>
    {
    private:
        std::atomic<bool> m_enabled;

    public:
        bool is_enabled() const noexcept { return m_enabled; }

#ifdef XAML_UI_GTK3
    private:
        static gboolean on_timeout(gpointer data);
#endif // XAML_UI_GTK3

    private:
        std::chrono::milliseconds m_interval;

    public:
        constexpr std::chrono::milliseconds get_interval() const noexcept { return m_interval; }
        void set_interval(std::chrono::milliseconds value)
        {
            bool enabled = m_enabled;
            stop();
            m_interval = value;
            if (enabled) start();
        }

    public:
        timer(std::chrono::milliseconds interval = std::chrono::milliseconds{ 1 }) : m_interval(interval) {}

    public:
        void start();
        void stop();

        EVENT(tick, timer const&)
    };
} // namespace xaml

#endif // !XAML_UI_TIMER_HPP
