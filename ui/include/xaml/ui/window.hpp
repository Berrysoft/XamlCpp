#ifndef XAML_UI_WINDOW_HPP
#define XAML_UI_WINDOW_HPP

#include <atomic>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/container.hpp>

#ifdef XAML_UI_WINDOWS
#include <wil/resource.h>
#endif // XAML_UI_WINDOWS

namespace xaml
{
#ifdef XAML_UI_WINDOWS
    class window;

    XAML_UI_API LRESULT CALLBACK __wnd_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
    XAML_UI_API std::shared_ptr<window> __get_window(HWND hWnd);
#endif // XAML_UI_WINDOWS

    class window : public container
    {
#ifdef XAML_UI_WINDOWS
    private:
        wil::unique_hdc_window m_store_dc{ nullptr };

    public:
        XAML_UI_API virtual std::optional<LRESULT> __wnd_proc(window_message const& msg) override;
        XAML_UI_API void __copy_hdc(rectangle const& region, HDC hDC);
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
    private:
        static void on_destroy(GtkWidget* w, gpointer arg);
        static gboolean on_configure_event(GtkWidget* widget, GdkEvent* event, gpointer data);
#endif // XAML_UI_GTK3

#ifdef XAML_UI_COCOA
    public:
        using __native_window_type = OBJC_OBJECT(NSWindow);

    private:
        __native_window_type m_window;

    public:
        inline __native_window_type __get_window() const { return m_window; }

    protected:
        void __set_window(__native_window_type value) { m_window = value; }

    public:
        void __on_did_resize();
        bool __on_should_close();
#endif // XAML_UI_COCOA

    public:
        XAML_UI_API void __draw(rectangle const& region) override;

    protected:
        XAML_UI_API void __parent_redraw() override;

    private:
        XAML_UI_API void draw_title();
        XAML_UI_API void draw_child();
        XAML_UI_API void draw_resizable();

    public:
        XAML_UI_API window();
        XAML_UI_API virtual ~window() override;

    public:
        std::shared_ptr<control> get_root_window() override { return shared_from_this(); }

        XAML_UI_API void show();

        PROP_EVENT(resizable, bool)
        EVENT(resizable_changed, window&, bool)

    private:
        std::atomic<bool> m_resizing{ false };

        EVENT(location_changed, window&, point)

        PROP_CONSTEXPR_EVENT(location, point)

    public:
        double get_x() const { return get_location().x; }
        double get_y() const { return get_location().y; }
        void set_x(double value)
        {
            if (m_location.x != value)
            {
                m_location.x = value;
                m_location_changed(*this, get_location());
            }
        }
        void set_y(double value)
        {
            if (m_location.y != value)
            {
                m_location.y = value;
                m_location_changed(*this, get_location());
            }
        }

        XAML_UI_API rectangle get_client_region() const;

        EVENT(closing, window&, bool&)

        EVENT(title_changed, window&, string_view_t)
        PROP_STRING_EVENT(title)

    public:
#define ADD_WINDOW_MEMBERS()  \
    ADD_CONTAINER_MEMBERS();  \
    ADD_PROP(title);          \
    ADD_EVENT(title_changed); \
    ADD_PROP_EVENT(location); \
    ADD_PROP(x);              \
    ADD_PROP(y);              \
    ADD_EVENT(closing);       \
    ADD_PROP_EVENT(resizable)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, window);
            ADD_WINDOW_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_WINDOW_HPP
