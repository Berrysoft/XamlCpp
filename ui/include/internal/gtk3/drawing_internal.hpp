#ifndef XAML_UI_DRAWING_INTERNAL_HPP
#define XAML_UI_DRAWING_INTERNAL_HPP

#include <gtk/gtk.h>
#include <xaml/ui/drawing.hpp>

namespace xaml
{
    constexpr rectangle get_rect(GdkRectangle const& rect) { return { (double)rect.x, (double)rect.y, (double)rect.width, (double)rect.height }; }
} // namespace xaml

#endif // !XAML_UI_DRAWING_INTERNAL_HPP
