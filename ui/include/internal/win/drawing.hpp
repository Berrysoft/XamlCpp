#ifndef XAML_UI_INTERNAL_WINDOWS_DRAWING_HPP
#define XAML_UI_INTERNAL_WINDOWS_DRAWING_HPP

#include <Windows.h>
#include <gdiplus.h>
#include <xaml/ui/drawing.hpp>

namespace xaml
{
    constexpr rectangle get_rect(RECT const& r) { return { (double)r.left, (double)r.top, (double)(r.right - r.left), (double)(r.bottom - r.top) }; }
    constexpr RECT get_RECT(rectangle const& r) { return { (LONG)r.x, (LONG)r.y, (LONG)(r.x + r.width), (LONG)(r.y + r.height) }; }

    inline Gdiplus::PointF get_PointF(point p) { return { (float)p.x, (float)p.y }; }
    inline Gdiplus::RectF get_RectF(rectangle const& r) { return { (float)r.x, (float)r.y, (float)r.width, (float)r.height }; }
} // namespace xaml

#endif // !XAML_UI_INTERNAL_WINDOWS_DRAWING_HPP
