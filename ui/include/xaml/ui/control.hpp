#ifndef XAML_UI_CONTROL_HPP
#define XAML_UI_CONTROL_HPP

#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/drawing.hpp>
#include <xaml/ui/strings.hpp>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#elif defined(XAML_UI_LINUX)
#include <gtk/gtk.h>
#endif

namespace xaml
{
    class container;
#ifdef XAML_UI_WINDOWS
    struct window_message
    {
        HWND hWnd;
        UINT Msg;
        WPARAM wParam;
        LPARAM lParam;
    };

    struct window_create_params
    {
        string_t class_name;
        string_t window_name;
        DWORD style;
        DWORD ex_style;
        int x;
        int y;
        int width;
        int height;
        container* parent;
    };

    LRESULT CALLBACK wnd_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
#endif

    class control : public std::enable_shared_from_this<control>
    {
#ifdef XAML_UI_WINDOWS
    private:
        HWND hWnd;

    public:
        constexpr HWND get_handle() const noexcept { return hWnd; }
        constexpr operator bool() const noexcept { return hWnd; }

    protected:
        void set_handle(HWND h) noexcept { hWnd = h; }

        void create(window_create_params const& params);

        virtual LRESULT wnd_proc(window_message const& msg);

        friend LRESULT CALLBACK wnd_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

#elif defined(XAML_UI_LINUX)
    private:
        GtkWidget* widget;

    public:
        constexpr GtkWidget* get_widget() const noexcept { return widget; }
        constexpr operator bool() const noexcept { return widget; }

    protected:
        void set_widget(GtkWidget* w) noexcept { widget = w; }
#endif

    private:
        std::shared_ptr<container> _parent;

    public:
        std::shared_ptr<container> get_parent() const { return _parent; }
        void set_parent(std::shared_ptr<container> const& value);

    public:
        control();
        virtual ~control();

        string_t get_text() const;
        void set_text(string_view_t value);

        point get_location() const;
        void set_location(point value);

        int get_x() const { return get_location().x; }
        int get_y() const { return get_location().y; }
        void set_x(int value) { set_location({ value, get_y() }); }
        void set_y(int value) { set_location({ get_x(), value }); }

        size get_size() const;
        void set_size(size value);

        int get_width() const { return get_size().width; }
        int get_height() const { return get_size().height; }
        void set_width(int value) { set_size({ value, get_height() }); }
        void set_height(int value) { set_size({ get_width(), value }); }
    };
} // namespace xaml

#endif // !XAML_UI_CONTROL_HPP
