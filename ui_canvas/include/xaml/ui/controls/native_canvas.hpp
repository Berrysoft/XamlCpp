#ifndef XAML_UI_CONTROLS_NATIVE_CANVAS_HPP
#define XAML_UI_CONTROLS_NATIVE_CANVAS_HPP

#include <xaml/ui/objc.hpp>

#ifdef XAML_UI_WINDOWS
#include <gdiplus.h>
#include <memory>
#include <wil/resource.h>
#elif defined(XAML_UI_GTK3)
#include <gtk/gtk.h>
#endif // XAML_UI_WINDOWS

namespace xaml
{
    struct native_drawing_context
    {
#ifdef XAML_UI_WINDOWS
        using native_handle_type = Gdiplus::Graphics*;
#elif defined(XAML_UI_GTK3)
        using native_handle_type = cairo_t*;
#elif defined(XAML_UI_COCOA)
        using native_handle_type = OBJC_OBJECT(NSGraphicsContext);
#endif

        native_handle_type handle{ OBJC_NIL };
    };

    struct native_canvas
    {
#ifdef XAML_UI_WINDOWS
        ULONG_PTR token;
        wil::unique_hdc_window store_dc;
#endif
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_NATIVE_CANVAS_HPP
