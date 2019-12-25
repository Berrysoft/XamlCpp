#ifndef XAML_UI_INTERNAL_GTK3_DRAWING_HPP
#define XAML_UI_INTERNAL_GTK3_DRAWING_HPP

#include <cmath>
#include <gtk/gtk.h>
#include <xaml/ui/drawing.hpp>

namespace xaml
{
    constexpr rectangle get_rect(GdkRectangle const& rect) { return { (double)rect.x, (double)rect.y, (double)rect.width, (double)rect.height }; }

    inline int get_rwidth(double request)
    {
        int r = (int)round(request);
        return r < 0 ? 0 : r;
    }

    inline int get_rheight(double request)
    {
        int r = (int)round(request);
        return r < 1 ? 1 : r;
    }
} // namespace xaml

#endif // !XAML_UI_INTERNAL_GTK3_DRAWING_HPP
