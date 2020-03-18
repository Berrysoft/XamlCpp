#ifndef XAML_UI_TIMER_HPP
#define XAML_UI_TIMER_HPP

#include <atomic>
#include <chrono>
#include <memory>
#include <xaml/meta/meta_macro.hpp>

namespace xaml
{
    struct native_timer;

    class timer
    {
    public:
        friend struct native_timer;

    public:
        using native_handle_type = std::shared_ptr<native_timer>;

    private:
        native_handle_type m_handle{ nullptr };

    public:
        native_handle_type get_handle() const noexcept { return m_handle; }

    protected:
        void set_handle(native_handle_type value) noexcept { m_handle = value; }

    private:
        std::atomic<bool> m_enabled{ false };

    public:
        bool is_enabled() const noexcept { return m_enabled; }

#ifdef XAML_UI_GTK3
    private:
        static int on_timeout(void* data);
#endif // XAML_UI_GTK3

#ifdef XAML_UI_COCOA
    public:
        void __on_tick();
#endif // XAML_UI_COCOA

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
        XAML_UI_API timer(std::chrono::milliseconds interval = std::chrono::milliseconds{ 1 });
        ~timer() { stop(); }

    public:
        XAML_UI_API void start();
        XAML_UI_API void stop();

        EVENT(tick, std::reference_wrapper<timer>)
    };
} // namespace xaml

#endif // !XAML_UI_TIMER_HPP
