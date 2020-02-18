#ifndef XAML_UI_WEBVIEW_WEBVIEW_HPP
#define XAML_UI_WEBVIEW_WEBVIEW_HPP

#include <functional>
#include <xaml/ui/control.hpp>

namespace xaml
{
#ifdef XAML_UI_WINDOWS
    struct native_webview
    {
        virtual ~native_webview() {}
        virtual void create_async(HWND parent, rectangle const& rect, std::function<void()>&& callback = {}) = 0;
        virtual operator bool() const = 0;
        virtual void navigate(string_view_t uri) = 0;
        virtual void set_location(point p) = 0;
        virtual void set_size(size s) = 0;
        virtual void set_rect(rectangle const& rect) = 0;
    };
#endif // XAML_UI_WINDOWS

    class webview : public control
    {
    public:
        XAML_UI_WEBVIEW_API webview();
        XAML_UI_WEBVIEW_API ~webview() override;

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

    public:
        XAML_UI_WEBVIEW_API void __draw(rectangle const& region) override;

    private:
        void draw_size();
        void draw_uri();

    public:
        EVENT(uri_changed, webview&, string_view_t)
        PROP_STRING_EVENT(uri)

    public:
#define ADD_WEBVIEW_MEMBERS() \
    ADD_CONTROL_MEMBERS();    \
    ADD_PROP_EVENT(uri)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, webview);
            ADD_CTOR_DEF();
            ADD_WEBVIEW_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_WEBVIEW_WEBVIEW_HPP
