#ifndef XAML_UI_CONTROLS_NATIVE_MENU_HPP
#define XAML_UI_CONTROLS_NATIVE_MENU_HPP

#include <xaml/ui/objc.hpp>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#include <wil/resource.h>
#elif defined(XAML_UI_GTK3)
#include <gtk/gtk.h>
#endif // XAML_UI_WINDOWS

namespace xaml
{
    struct native_menu_item
    {
#ifdef XAML_UI_WINDOWS
        using native_handle_type = UINT;
        using native_parent_type = HMENU;
#elif defined(XAML_UI_GTK3)
        using native_handle_type = GtkMenuItem*;
        using native_parent_type = GtkMenuBar*;
#elif defined(XAML_UI_COCOA)
        using native_handle_type = OBJC_OBJECT(NSMenuItem);
        using native_parent_type = OBJC_OBJECT(NSMenu);
#endif // XAML_UI_WINDOWS

        native_handle_type handle{};
        native_parent_type parent{ OBJC_NIL };
    };

    struct native_popup_menu_item : native_menu_item
    {
#ifdef XAML_UI_WINDOWS
        using native_menu_type = wil::unique_hmenu;

        native_menu_type menu{ nullptr };
#endif // XAML_UI_WINDOWS
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_NATIVE_MENU_HPP
