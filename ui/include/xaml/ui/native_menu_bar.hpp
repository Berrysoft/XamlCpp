#ifndef XAML_UI_NATIVE_MENU_BAR_HPP
#define XAML_UI_NATIVE_MENU_BAR_HPP

#include <xaml/ui/objc.hpp>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#include <wil/resource.h>
#elif defined(XAML_UI_GTK3)
#include <gtk/gtk.h>
#endif // XAML_UI_WINDOWS

namespace xaml
{
    struct native_menu_bar
    {
#ifdef XAML_UI_WINDOWS
        using native_handle_type = wil::unique_hmenu;
#elif defined(XAML_UI_GTK3)
        using native_handle_type = GtkMenuBar*;
#elif defined(XAML_UI_COCOA)
        using native_handle_type = OBJC_OBJECT(NSMenu);
#endif // XAML_UI_WINDOWS

        native_handle_type handle{ OBJC_NIL };
    };
} // namespace xaml

#endif // !XAML_UI_NATIVE_MENU_BAR_HPP
