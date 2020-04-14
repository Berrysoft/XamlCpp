#ifndef XAML_UI_NATIVE_DRAWING_HPP
#define XAML_UI_NATIVE_DRAWING_HPP

#include <xaml/ui/drawing.hpp>
#include <xaml/ui/objc.hpp>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#elif defined(XAML_UI_GTK3)
#include <gtk/gtk.h>
#endif // XAML_UI_WINDOWS

namespace xaml
{
    template <typename To, typename From>
    inline To to_native(From) noexcept = delete;

#ifdef XAML_UI_WINDOWS
    constexpr size from_native(SIZE s) noexcept
    {
        return { (double)s.cx, (double)s.cy };
    }
    template <>
    constexpr SIZE to_native<SIZE, size>(size s) noexcept
    {
        return { (LONG)s.width, (LONG)s.height };
    }

    constexpr point from_native(POINT p) noexcept
    {
        return { (double)p.x, (double)p.y };
    }
    template <>
    constexpr POINT to_native<POINT, point>(point p) noexcept
    {
        return { (LONG)p.x, (LONG)p.y };
    }

    constexpr rectangle from_native(RECT r) noexcept
    {
        return { (double)r.left, (double)r.top, (double)r.right - r.left, (double)r.bottom - r.top };
    }
    template <>
    constexpr RECT to_native<RECT, rectangle>(rectangle r) noexcept
    {
        return { (LONG)r.x, (LONG)r.y, (LONG)(r.x + r.width), (LONG)(r.y + r.height) };
    }
#elif defined(XAML_UI_GTK3)
    constexpr size from_native(gint width, gint height) noexcept
    {
        return { (double)width, (double)height };
    }
    template <>
    inline std::tuple<gint, gint> to_native<std::tuple<gint, gint>, size>(size s) noexcept
    {
        return std::make_tuple((std::max)(0, (int)s.width), (std::max)(1, (int)s.height));
    }

    constexpr point from_native(GdkPoint p) noexcept
    {
        return { (double)p.x, (double)p.y };
    }
    template <>
    constexpr GdkPoint to_native<GdkPoint, point>(point p) noexcept
    {
        return { (int)p.x, (int)p.y };
    }

    constexpr rectangle from_native(GdkRectangle const& r) noexcept
    {
        return { (double)r.x, (double)r.y, (double)r.width, (double)r.height };
    }
    template <>
    constexpr GdkRectangle to_native<GdkRectangle, rectangle>(rectangle r) noexcept
    {
        return { (int)r.x, (int)r.y, (int)r.width, (int)r.height };
    }
#elif defined(XAML_UI_COCOA) && defined(__OBJC__)
    constexpr size from_native(NSSize s) noexcept
    {
        return { s.width, s.height };
    }
    template <>
    constexpr NSSize to_native<NSSize, size>(size s) noexcept
    {
        return { s.width, s.height };
    }

    constexpr point from_native(NSPoint p) noexcept
    {
        return { p.x, p.y };
    }
    template <>
    constexpr NSPoint to_native<NSPoint, point>(point p) noexcept
    {
        return { p.x, p.y };
    }

    constexpr rectangle from_native(NSRect const& r) noexcept
    {
        return { r.origin.x, r.origin.y, r.size.width, r.size.height };
    }
    template <>
    constexpr NSRect to_native<NSRect, rectangle>(rectangle r) noexcept
    {
        return { { r.x, r.y }, { r.width, r.height } };
    }
#endif // XAML_UI_WINDOWS
} // namespace xaml

#endif // !XAML_UI_NATIVE_DRAWING_HPP
