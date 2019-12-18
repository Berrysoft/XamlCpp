#ifndef XAML_UI_CONTROL_HPP
#define XAML_UI_CONTROL_HPP

#include <xaml/meta/meta_macro.hpp>
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
        friend class container;
        friend class window;

#ifdef XAML_UI_WINDOWS
    private:
        HWND hWnd;

    public:
        constexpr HWND get_handle() const noexcept { return hWnd; }
        constexpr operator bool() const noexcept { return hWnd; }

    protected:
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
        void create(GtkWidget* w) { widget = w; }
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
    };
} // namespace xaml

#endif // !XAML_UI_CONTROL_HPP
