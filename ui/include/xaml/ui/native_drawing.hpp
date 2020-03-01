#ifndef XAML_UI_NATIVE_DRAWING_HPP
#define XAML_UI_NATIVE_DRAWING_HPP

#include <xaml/ui/drawing.hpp>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#include <gdiplus.h>
#elif defined(XAML_UI_GTK3)
#include <gtk/gtk.h>
#elif defined(XAML_UI_COCOA)
#include <xaml/ui/objc.hpp>
#endif // XAML_UI_WINDOWS

namespace xaml
{
#ifdef XAML_UI_WINDOWS
    constexpr size from_native(SIZE s)
    {
        return { (double)s.cx, (double)s.cy };
    }
    inline size from_native(Gdiplus::SizeF s)
    {
        return { (double)s.Width, (double)s.Height };
    }
    template <>
    constexpr SIZE to_native<SIZE, size>(size s)
    {
        return { (LONG)s.width, (LONG)s.height };
    }
    template <>
    inline Gdiplus::SizeF to_native<Gdiplus::SizeF, size>(size s)
    {
        return { (float)s.width, (float)s.height };
    }

    constexpr point from_native(POINT p)
    {
        return { (double)p.x, (double)p.y };
    }
    inline point from_native(Gdiplus::PointF p)
    {
        return { (double)p.X, (double)p.Y };
    }
    template <>
    constexpr POINT to_native<POINT, point>(point p)
    {
        return { (LONG)p.x, (LONG)p.y };
    }
    template <>
    inline Gdiplus::PointF to_native<Gdiplus::PointF, point>(point p)
    {
        return { (float)p.x, (float)p.y };
    }

    constexpr rectangle from_native(RECT r)
    {
        return { (double)r.left, (double)r.top, (double)r.right - r.left, (double)r.bottom - r.top };
    }
    inline rectangle from_native(Gdiplus::RectF r)
    {
        return { (double)r.X, (double)r.Y, (double)r.Width, (double)r.Height };
    }
    template <>
    constexpr RECT to_native<RECT, rectangle>(rectangle r)
    {
        return { (LONG)r.x, (LONG)r.y, (LONG)(r.x + r.width), (LONG)(r.y + r.height) };
    }
    template <>
    inline Gdiplus::RectF to_native<Gdiplus::RectF, rectangle>(rectangle r)
    {
        return { (float)r.x, (float)r.y, (float)r.width, (float)r.height };
    }
#elif defined(XAML_UI_GTK3)
    constexpr size from_native(gint width, gint height)
    {
        return { (double)width, (double)height };
    }
    template <>
    inline std::tuple<gint, gint> to_native<std::tuple<gint, gint>, size>(size s)
    {
        return std::make_tuple((std::max)(0, (int)s.width), (std::max)(1, (int)s.height));
    }

    constexpr point from_native(GdkPoint p)
    {
        return { (double)p.x, (double)p.y };
    }
    template <>
    constexpr GdkPoint to_native<GdkPoint, point>(point p)
    {
        return { (int)p.x, (int)p.y };
    }

    constexpr rectangle from_native(GdkRectangle const& r)
    {
        return { (double)r.x, (double)r.y, (double)r.width, (double)r.height };
    }
    template <>
    constexpr GdkRectangle to_native<GdkRectangle, rectangle>(rectangle r)
    {
        return { (int)r.x, (int)r.y, (int)r.width, (int)r.height };
    }
#elif defined(XAML_UI_COCOA) && defined(__OBJC__)
    constexpr size from_native(NSSize s)
    {
        return { s.width, s.height };
    }
    template <>
    constexpr NSSize to_native<NSSize, size>(size s)
    {
        return { s.width, s.height };
    }

    constexpr point from_native(NSPoint p)
    {
        return { p.x, p.y };
    }
    template <>
    constexpr NSPoint to_native<NSPoint, point>(point p)
    {
        return { p.x, p.y };
    }

    constexpr rectangle from_native(NSRect const& r)
    {
        return { r.origin.x, r.origin.y, r.size.width, r.size.height };
    }
    template <>
    constexpr NSRect to_native<NSRect, rectangle>(rectangle r)
    {
        return { { r.x, r.y }, { r.width, r.height } };
    }
#endif // XAML_UI_WINDOWS
} // namespace xaml

#endif // !XAML_UI_NATIVE_DRAWING_HPP
