#ifndef XAML_UI_CANVAS_SHARED_POINT_HPP
#define XAML_UI_CANVAS_SHARED_POINT_HPP

#include <cmath>
#include <xaml/ui/drawing.h>

inline xaml_point lerp_point(xaml_rectangle const& region, xaml_point const& p) noexcept
{
    return { std::lerp(region.x, region.x + region.width, p.x), std::lerp(region.y, region.y + region.height, p.y) };
}

#endif // !XAML_UI_CANVAS_SHARED_POINT_HPP
