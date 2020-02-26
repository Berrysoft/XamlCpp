#ifndef XAML_UI_WEBVIEW_WEBVIEW_HPP
#define XAML_UI_WEBVIEW_WEBVIEW_HPP

#include <atomic>
#include <functional>
#include <optional>
#include <xaml/array_view.hpp>
#include <xaml/ui/control.hpp>

#ifdef XAML_UI_GTK3
#include <webkit2/webkit2.h>
#endif

namespace xaml
{
    struct web_request
    {
        string_view_t method;
        string_view_t uri;
        array_view<std::byte> data;
    };

    struct web_response
    {
        string_view_t content_type;
        array_view<std::byte> data;
    };

    struct resource_requested_args
    {
        web_request request;
        std::optional<web_response> response;
    };

#ifdef XAML_UI_WINDOWS
    struct native_webview
    {
    public:
        virtual ~native_webview() {}
        virtual void create_async(HWND parent, rectangle const& rect, std::function<void()>&& callback = {}) = 0;
        virtual operator bool() const = 0;
        virtual void navigate(string_view_t uri) = 0;
        virtual void set_location(point p) = 0;
        virtual void set_size(size s) = 0;
        virtual void set_rect(rectangle const& rect) = 0;

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

    class webview : public control
    {
    public:
        XAML_UI_WEBVIEW_API webview();
        XAML_UI_WEBVIEW_API ~webview() override;

    private:
        std::atomic<bool> m_navigating{ false };

#ifdef XAML_UI_WINDOWS
    public:
        using __native_webview_type = std::shared_ptr<native_webview>;

    private:
        __native_webview_type m_webview{ OBJC_NIL };

    public:
        __native_webview_type __get_webview() const noexcept { return m_webview; }

    protected:
        void __set_webview(__native_webview_type value) OBJC_BLOCK({ m_webview = value; });
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
    private:
        static void on_load_changed(WebKitWebView* web_view, WebKitLoadEvent load_event, gpointer data);
#endif // XAML_UI_GTK3

#ifdef XAML_UI_COCOA
    public:
        void __on_navigated();
#endif // XAML_UI_COCOA

    public:
        XAML_UI_WEBVIEW_API void __draw(rectangle const& region) override;

#ifdef XAML_UI_WINDOWS
    private:
        void draw_create();
#endif // XAML_UI_WINDOWS

    private:
        void draw_size();
        void draw_uri();

    public:
        EVENT(uri_changed, webview&, string_view_t)
        PROP_STRING_EVENT(uri)

        EVENT(resource_requested, webview&, resource_requested_args&)

    public:
#define ADD_WEBVIEW_MEMBERS() \
    ADD_CONTROL_MEMBERS();    \
    ADD_PROP_EVENT(uri)

        REGISTER_CLASS_DECL()
        {
            REGISTER_TYPE(xaml, webview);
            ADD_CTOR_DEF();
            ADD_WEBVIEW_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_WEBVIEW_WEBVIEW_HPP
