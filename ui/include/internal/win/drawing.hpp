#ifndef XAML_UI_INTERNAL_WINDOWS_DRAWING_HPP
#define XAML_UI_INTERNAL_WINDOWS_DRAWING_HPP

#include <xaml/ui/drawing.hpp>
#include <Windows.h>

namespace xaml
{
    constexpr rectangle get_rect(RECT const& r) { return { (double)r.left, (double)r.top, (double)(r.right - r.left), (double)(r.bottom - r.top) }; }
}

#endif // !XAML_UI_INTERNAL_WINDOWS_DRAWING_HPP
