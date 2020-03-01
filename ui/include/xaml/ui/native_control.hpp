#ifndef XAML_UI_NATIVE_CONTROL_HPP
#define XAML_UI_NATIVE_CONTROL_HPP

#include <xaml/ui/objc.hpp>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#elif defined(XAML_UI_GTK3)
#include <gtk/gtk.h>
#elif defined(XAML_UI_COCOA) && defined(__OBJC__)
#import <xaml/ui/cocoa/XamlDelegate.h>
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
#endif

    struct native_control
    {
    public:
#ifdef XAML_UI_WINDOWS
        using native_handle_type = HWND;
#elif defined(XAML_UI_GTK3)
        using native_handle_type = GtkWidget*;
#elif defined(XAML_UI_COCOA)
        using native_handle_type = OBJC_OBJECT(NSView);
#endif // XAML_UI_WINDOWS

    public:
        native_handle_type handle{ OBJC_NIL };

#ifdef XAML_UI_COCOA
    public:
        using native_delegate_type = OBJC_OBJECT(XamlDelegate);

    public:
        native_delegate_type delegate{ OBJC_NIL };
#endif // XAML_UI_COCOA
    };
} // namespace xaml

#endif // !XAML_UI_NATIVE_CONTROL_HPP
