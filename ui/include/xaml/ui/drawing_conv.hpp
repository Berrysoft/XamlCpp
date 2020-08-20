#ifndef XAML_UI_NATIVE_DRAWING_HPP
#define XAML_UI_NATIVE_DRAWING_HPP

#include <xaml/ui/drawing.h>

#ifdef XAML_UI_WINDOWS
    #include <Windows.h>
#elif defined(XAML_UI_COCOA)
    #include <xaml/ui/cocoa/objc.h>
#elif defined(XAML_UI_GTK3)
    #include <gtk/gtk.h>
#elif defined(XAML_UI_QT5)
    #include <QPoint>
    #include <QRect>
    #include <QSize>
#endif // XAML_UI_WINDOWS

template <typename To, typename From>
inline To xaml_to_native(From const&) noexcept = delete;

#ifdef XAML_UI_WINDOWS
constexpr xaml_size xaml_from_native(SIZE const& s) noexcept
{
    return { (double)s.cx, (double)s.cy };
}
template <>
constexpr SIZE xaml_to_native<SIZE, xaml_size>(xaml_size const& s) noexcept
{
    return { (LONG)s.width, (LONG)s.height };
}

constexpr xaml_point xaml_from_native(POINT const& p) noexcept
{
    return { (double)p.x, (double)p.y };
}
template <>
constexpr POINT xaml_to_native<POINT, xaml_point>(xaml_point const& p) noexcept
{
    return { (LONG)p.x, (LONG)p.y };
}

constexpr xaml_rectangle xaml_from_native(RECT const& r) noexcept
{
    return { (double)r.left, (double)r.top, (double)r.right - r.left, (double)r.bottom - r.top };
}
template <>
constexpr RECT xaml_to_native<RECT, xaml_rectangle>(xaml_rectangle const& r) noexcept
{
    return { (LONG)r.x, (LONG)r.y, (LONG)(r.x + r.width), (LONG)(r.y + r.height) };
}
#elif defined(XAML_UI_GTK3)
constexpr xaml_size xaml_from_native(gint width, gint height) noexcept
{
    return { (double)width, (double)height };
}
template <>
inline std::tuple<gint, gint> xaml_to_native<std::tuple<gint, gint>, xaml_size>(xaml_size const& s) noexcept
{
    return std::make_tuple((std::max)(0, (int)s.width), (std::max)(1, (int)s.height));
}

constexpr xaml_point xaml_from_native(GdkPoint const& p) noexcept
{
    return { (double)p.x, (double)p.y };
}
template <>
constexpr GdkPoint xaml_to_native<GdkPoint, xaml_point>(xaml_point const& p) noexcept
{
    return { (int)p.x, (int)p.y };
}

constexpr xaml_rectangle xaml_from_native(GdkRectangle const& r) noexcept
{
    return { (double)r.x, (double)r.y, (double)r.width, (double)r.height };
}
template <>
constexpr GdkRectangle xaml_to_native<GdkRectangle, xaml_rectangle>(xaml_rectangle const& r) noexcept
{
    return { (int)r.x, (int)r.y, (int)r.width, (int)r.height };
}
#elif defined(XAML_UI_COCOA) && defined(__OBJC__)
constexpr xaml_size xaml_from_native(NSSize const& s) noexcept
{
    return { s.width, s.height };
}
template <>
constexpr NSSize xaml_to_native<NSSize, xaml_size>(xaml_size const& s) noexcept
{
    return { s.width, s.height };
}

constexpr xaml_point xaml_from_native(NSPoint const& p) noexcept
{
    return { p.x, p.y };
}
template <>
constexpr NSPoint xaml_to_native<NSPoint, xaml_point>(xaml_point const& p) noexcept
{
    return { p.x, p.y };
}

constexpr xaml_rectangle xaml_from_native(NSRect const& r) noexcept
{
    return { r.origin.x, r.origin.y, r.size.width, r.size.height };
}
template <>
constexpr NSRect xaml_to_native<NSRect, xaml_rectangle>(xaml_rectangle const& r) noexcept
{
    return { { r.x, r.y }, { r.width, r.height } };
}

template <>
inline NSColor* xaml_to_native<NSColor*, xaml_color>(xaml_color const& c) noexcept
{
    return [NSColor colorWithCalibratedRed:(c.r / 255.0) green:(c.g / 255.0) blue:(c.b / 255.0) alpha:(c.a / 255.0)];
}
#elif defined(XAML_UI_QT5)
constexpr xaml_size xaml_from_native(QSize const& s) noexcept
{
    return { (double)s.width(), (double)s.height() };
}
template <>
constexpr QSize xaml_to_native<QSize, xaml_size>(xaml_size const& s) noexcept
{
    return { (int)s.width, (int)s.height };
}

constexpr xaml_point xaml_from_native(QPoint const& p) noexcept
{
    return { (double)p.x(), (double)p.y() };
}
template <>
constexpr QPoint xaml_to_native<QPoint, xaml_point>(xaml_point const& p) noexcept
{
    return { (int)p.x, (int)p.y };
}

constexpr xaml_rectangle xaml_from_native(QRect const& r) noexcept
{
    return { (double)r.left(), (double)r.top(), (double)r.width(), (double)r.height() };
}
template <>
constexpr QRect xaml_to_native<QRect, xaml_rectangle>(xaml_rectangle const& r) noexcept
{
    return { (int)r.x, (int)r.y, (int)(r.width), (int)(r.height) };
}
#endif // XAML_UI_WINDOWS

#endif // !XAML_UI_NATIVE_DRAWING_HPP
