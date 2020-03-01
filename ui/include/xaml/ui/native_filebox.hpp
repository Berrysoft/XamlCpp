#ifndef XAML_UI_NATIVE_FILEBOX_HPP
#define XAML_UI_NATIVE_FILEBOX_HPP

#ifdef XAML_UI_WINDOWS
#include <Windows.h>

#include <ShObjIdl.h>
#include <wil/com.h>
#elif defined(XAML_UI_GTK3)
#include <gtk/gtk.h>
#elif defined(XAML_UI_COCOA) && defined(__OBJC__)
#import <xaml/ui/cocoa/XamlDelegate.h>
#endif

namespace xaml
{
    struct native_filebox
    {
#ifdef XAML_UI_WINDOWS
        using native_handle_type = wil::com_ptr<IFileDialog>;
#elif defined(XAML_UI_GTK3)
        using native_handle_type = GtkWidget*;
#elif defined(XAML_UI_COCOA)
        using native_handle_type = OBJC_OBJECT(NSSavePanel);
#endif // XAML_UI_WINDOWS

        native_handle_type handle{ OBJC_NIL };
    };
} // namespace xaml

#endif // !XAML_UI_NATIVE_FILEBOX_HPP
