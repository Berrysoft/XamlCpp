#ifndef XAML_UI_CONTROLS_NATIVE_CANVAS_HPP
#define XAML_UI_CONTROLS_NATIVE_CANVAS_HPP

#include <xaml/ui/objc.hpp>

#ifdef XAML_UI_WINDOWS
#ifdef XAML_UI_CANVAS_DIRECT2D
#include <d2d1.h>
#include <dwrite.h>
#include <wil/com.h>
#else
#include <gdiplus.h>
#endif // XAML_UI_CANVAS_DIRECT2D
#include <wil/resource.h>
#include <xaml/ui/native_drawing.hpp>
#elif defined(XAML_UI_GTK3)
#include <gtk/gtk.h>
#endif // XAML_UI_WINDOWS

namespace xaml
{
    struct native_drawing_context
    {
#ifdef XAML_UI_WINDOWS
#ifdef XAML_UI_CANVAS_DIRECT2D
        using native_handle_type = wil::com_ptr<ID2D1RenderTarget>;
#else
        using native_handle_type = Gdiplus::Graphics*;
#endif // XAML_UI_CANVAS_DIRECT2D
#elif defined(XAML_UI_GTK3)
        using native_handle_type = cairo_t*;
#elif defined(XAML_UI_COCOA)
        using native_handle_type = OBJC_OBJECT(NSGraphicsContext);
#endif

        native_handle_type handle{ OBJC_NIL };

#if defined(XAML_UI_WINDOWS) && defined(XAML_UI_CANVAS_DIRECT2D)
        using native_dwrite_type = wil::com_ptr<IDWriteFactory>;

        native_dwrite_type dwrite{ nullptr };
#endif // XAML_UI_WINDOWS && XAML_UI_CANVAS_DIRECT2D
    };

    struct native_canvas
    {
#ifdef XAML_UI_WINDOWS
#ifdef XAML_UI_CANVAS_DIRECT2D
        wil::com_ptr<ID2D1Factory> factory;
#else
        ULONG_PTR token;
#endif // XAML_UI_CANVAS_DIRECT2D
        wil::unique_hdc_window store_dc;
#endif
    };

#ifdef XAML_UI_WINDOWS
#ifdef XAML_UI_CANVAS_DIRECT2D
    inline size from_native(D2D1_SIZE_F s)
    {
        return { (double)s.width, (double)s.height };
    }
    template <>
    inline D2D1_SIZE_F to_native<D2D1_SIZE_F, size>(size s)
    {
        return { (float)s.width, (float)s.height };
    }

    inline point from_native(D2D1_POINT_2F p)
    {
        return { (double)p.x, (double)p.y };
    }
    template <>
    inline D2D1_POINT_2F to_native<D2D1_POINT_2F, point>(point p)
    {
        return { (float)p.x, (float)p.y };
    }

    inline rectangle from_native(D2D1_RECT_F r)
    {
        return { (double)r.left, (double)r.top, (double)(r.right - r.left), (double)(r.bottom - r.top) };
    }
    template <>
    inline D2D1_RECT_F to_native<D2D1_RECT_F, rectangle>(rectangle r)
    {
        return { (float)r.x, (float)r.y, (float)(r.x + r.width), (float)(r.y + r.height) };
    }
#else
    inline size from_native(Gdiplus::SizeF s)
    {
        return { (double)s.Width, (double)s.Height };
    }
    template <>
    inline Gdiplus::SizeF to_native<Gdiplus::SizeF, size>(size s)
    {
        return { (float)s.width, (float)s.height };
    }

    inline point from_native(Gdiplus::PointF p)
    {
        return { (double)p.X, (double)p.Y };
    }
    template <>
    inline Gdiplus::PointF to_native<Gdiplus::PointF, point>(point p)
    {
        return { (float)p.x, (float)p.y };
    }

    inline rectangle from_native(Gdiplus::RectF r)
    {
        return { (double)r.X, (double)r.Y, (double)r.Width, (double)r.Height };
    }
    template <>
    inline Gdiplus::RectF to_native<Gdiplus::RectF, rectangle>(rectangle r)
    {
        return { (float)r.x, (float)r.y, (float)r.width, (float)r.height };
    }
#endif // XAML_UI_CANVAS_DIRECT2D
#endif // XAML_UI_WINDOWS

} // namespace xaml

#endif // !XAML_UI_CONTROLS_NATIVE_CANVAS_HPP
