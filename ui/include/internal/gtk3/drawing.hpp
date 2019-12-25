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

    constexpr cairo_font_weight_t get_font_weight(font_weight weight)
    {
        if ((std::int32_t)weight < (std::int32_t)font_weight::semi_bold)
            return CAIRO_FONT_WEIGHT_NORMAL;
        else
            return CAIRO_FONT_WEIGHT_BOLD;
    }
} // namespace xaml

#endif // !XAML_UI_INTERNAL_GTK3_DRAWING_HPP
