#ifndef XAML_UI_WEBVIEW_WEBVIEW_HPP
#define XAML_UI_WEBVIEW_WEBVIEW_HPP

#include <atomic>
#include <optional>
#include <xaml/array_view.hpp>
#include <xaml/ui/control.hpp>

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

    struct resource_requested_args;

    template <>
    struct type_guid<resource_requested_args>
    {
        static constexpr guid value{ 0x507c59c1, 0x5775, 0x484e, 0x9c, 0xe5, 0xbc, 0x34, 0x23, 0x85, 0x5a, 0x0b };
    };

    struct resource_requested_args : public meta_class
    {
        META_CLASS_IMPL(meta_class)

        web_request request;
        std::optional<web_response> response;
    };

#ifdef XAML_UI_WINDOWS
    struct native_webview;
#endif // XAML_UI_WINDOWS
    class webview;

    template <>
    struct type_guid<webview>
    {
        static constexpr guid value{ 0xb39028bb, 0xc65f, 0x4df9, 0xa0, 0xef, 0xf2, 0x04, 0x30, 0x77, 0x40, 0xda };
    };

    class webview : public control
    {
    public:
        META_CLASS_IMPL(control)

    public:
        XAML_UI_WEBVIEW_API webview();
        XAML_UI_WEBVIEW_API ~webview() override;

    private:
        std::atomic_bool m_navigating{ false };

#ifdef XAML_UI_WINDOWS
    public:
        using native_webview_type = std::shared_ptr<native_webview>;

    private:
        native_webview_type m_webview{ nullptr };

    public:
        native_webview_type get_webview() const noexcept { return m_webview; }

    protected:
        void set_webview(native_webview_type value) { m_webview = value; }

    private:
        std::atomic_bool m_created{ false };

        void create_edge2(rectangle const& real);
        void create_edge(rectangle const& real);
        void create_ie(rectangle const& real);
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
    private:
        static void on_load_changed(void* web_view, int load_event, void* data);
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

    protected:
        XAML_UI_WEBVIEW_API void draw_size() override;
        XAML_UI_WEBVIEW_API void draw_visible() override;
#endif // XAML_UI_WINDOWS

    protected:
        XAML_UI_WEBVIEW_API virtual void draw_uri();

    public:
        EVENT(uri_changed, std::shared_ptr<webview>, string_view_t)
        PROP_STRING_EVENT(uri)

        XAML_UI_WEBVIEW_API bool get_can_go_forward();
        XAML_UI_WEBVIEW_API bool get_can_go_back();

        XAML_UI_WEBVIEW_API void go_forward();
        XAML_UI_WEBVIEW_API void go_back();

        EVENT(resource_requested, std::shared_ptr<webview>, std::shared_ptr<resource_requested_args>)

    public:
#define ADD_WEBVIEW_MEMBERS() \
    ADD_CONTROL_MEMBERS();    \
    ADD_PROP_EVENT(uri)

        REGISTER_CLASS_DECL(xaml, webview, "xaml/ui/controls")
        {
            ADD_CTOR_DEF();
            ADD_WEBVIEW_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_WEBVIEW_WEBVIEW_HPP
