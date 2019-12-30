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

    LRESULT CALLBACK __wnd_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
    std::shared_ptr<window> __get_window(HWND hWnd);
#endif // XAML_UI_WINDOWS

    class window : public container /*, public meta_class_impl<window>*/
    {
#ifdef XAML_UI_WINDOWS
    private:
        wil::unique_hdc_window m_store_dc{ nullptr };

    public:
        virtual std::optional<LRESULT> __wnd_proc(window_message const& msg) override;
        void __copy_hdc(rectangle const& region, HDC hDC);
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
        void __draw(rectangle const& region) override;

    protected:
        void __parent_redraw() override;

    private:
        void draw_title();
        void draw_child();
        void draw_resizable();

    public:
        window();
        virtual ~window() override;

    public:
        std::shared_ptr<control> get_root_window() override { return shared_from_this(); }

        void show();

        PROP_EVENT(resizable, bool)
        EVENT(resizable_changed, window const&, bool)

    private:
        std::atomic<bool> m_resizing{ false };

        point m_location{ 0, 0 };

        EVENT(location_changed, window const&, point)

    public:
        constexpr point get_location() const noexcept { return m_location; }
        void set_location(point value)
        {
            if (m_location != value)
            {
                m_location = value;
                m_location_changed(*this, get_location());
            }
        }

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

        rectangle get_client_region() const;

        EVENT(closing, window const&, bool&)

    private:
        string_t m_title{};

        EVENT(title_changed, window const&, string_view_t)

    public:
        string_view_t get_title() const noexcept { return m_title; }
        void set_title(string_view_t value) { m_title = (string_t)value; }

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
