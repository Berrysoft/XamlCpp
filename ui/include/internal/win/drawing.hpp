#ifndef XAML_UI_INTERNAL_WINDOWS_DRAWING_HPP
#define XAML_UI_INTERNAL_WINDOWS_DRAWING_HPP

#include <Windows.h>
#include <xaml/ui/drawing.hpp>

namespace xaml
{
    constexpr rectangle get_rect(RECT const& r) { return { (double)r.left, (double)r.top, (double)(r.right - r.left), (double)(r.bottom - r.top) }; }
    constexpr RECT get_RECT(rectangle const& r) { return { (LONG)r.x, (LONG)r.y, (LONG)(r.x + r.width), (LONG)(r.y + r.height) }; }
} // namespace xaml

#endif // !XAML_UI_INTERNAL_WINDOWS_DRAWING_HPP
