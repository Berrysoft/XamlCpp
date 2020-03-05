#ifndef XAML_UI_NATIVE_WINDOW_HPP
#define XAML_UI_NATIVE_WINDOW_HPP

#ifdef XAML_UI_WINDOWS
#include <wil/resource.h>
#elif defined(XAML_UI_GTK3)
#include <gtk/gtk.h>
#elif defined(XAML_UI_COCOA) && defined(__OBJC__)
#import <xaml/ui/cocoa/XamlDelegate.h>
#endif // XAML_UI_WINDOWS

namespace xaml
{
    struct native_window
    {
#ifdef XAML_UI_WINDOWS
        wil::unique_hdc_window store_dc{ nullptr };
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
        GtkWidget* vbox;
        GtkWidget* menu_bar;
#endif // XAML_UI_GTK3

#ifdef XAML_UI_COCOA
        using native_window_type = OBJC_OBJECT(NSWindow);

        native_window_type window;
#endif // XAML_UI_COCOA
    };

#ifdef XAML_UI_WINDOWS
    class window;

    XAML_UI_API LRESULT CALLBACK __wnd_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
    XAML_UI_API std::shared_ptr<window> __get_window(HWND hWnd);
#endif // XAML_UI_WINDOWS
} // namespace xaml

#endif // !XAML_UI_NATIVE_WINDOW_HPP
