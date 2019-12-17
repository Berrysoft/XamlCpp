#ifndef XAML_UI_CONTROL_HPP
#define XAML_UI_CONTROL_HPP

#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/strings.hpp>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#endif

namespace xaml
{
#ifdef XAML_UI_WINDOWS
    class control;

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
        control* parent;
    };

    LRESULT CALLBACK wnd_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
#endif

    class control : meta_class_impl<control>
    {
#ifdef XAML_UI_WINDOWS
    private:
        HWND hWnd;

    public:
        constexpr HWND handle() const noexcept { return hWnd; }
        constexpr operator bool() const noexcept { return hWnd; }

    protected:
        void create(window_create_params const& params);

        virtual LRESULT wnd_proc(window_message const& msg);

        friend LRESULT CALLBACK wnd_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
#endif

    public:
        control();
        virtual ~control();
    };
} // namespace xaml

#endif // !XAML_UI_CONTROL_HPP
