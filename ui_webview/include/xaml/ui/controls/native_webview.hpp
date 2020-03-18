#ifndef XAML_UI_WEBVIEW_NATIVE_WEBVIEW_HPP
#define XAML_UI_WEBVIEW_NATIVE_WEBVIEW_HPP

#include <functional>
#include <xaml/ui/controls/webview.hpp>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#endif // XAML_UI_WINDOWS

namespace xaml
{
#ifdef XAML_UI_WINDOWS
    struct native_webview
    {
    public:
        virtual ~native_webview() {}
        virtual void create_async(HWND parent, rectangle const& rect, std::function<void()>&& callback = {}) = 0;
        virtual operator bool() const = 0;
        virtual void navigate(string_view_t uri) = 0;
        virtual void set_visible(bool vis) = 0;
        virtual void set_location(point p) = 0;
        virtual void set_size(size s) = 0;
        virtual void set_rect(rectangle const& rect) = 0;
        virtual bool get_can_go_forward() = 0;
        virtual void go_forward() = 0;
        virtual bool get_can_go_back() = 0;
        virtual void go_back() = 0;

#define __NATIVE_WEBVIEW_EVENT(name, arg)                                                    \
private:                                                                                     \
    std::function<void(arg)> m_##name{};                                                     \
                                                                                             \
public:                                                                                      \
    void set_##name(std::function<void(arg)>&& callback) { m_##name = std::move(callback); } \
    void invoke_##name(arg a)                                                                \
    {                                                                                        \
        if (m_##name) m_##name(a);                                                           \
    }

        __NATIVE_WEBVIEW_EVENT(navigated, string_view_t)
        __NATIVE_WEBVIEW_EVENT(resource_requested, resource_requested_args&)

#undef __NATIVE_WEBVIEW_EVENT
    };
#endif // XAML_UI_WINDOWS
} // namespace xaml

#endif // !XAML_UI_WEBVIEW_NATIVE_WEBVIEW_HPP
