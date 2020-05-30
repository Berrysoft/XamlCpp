#ifndef XAML_UI_CANVAS_SHARED_POINT_HPP
#define XAML_UI_CANVAS_SHARED_POINT_HPP

#include <xaml/ui/drawing.h>

constexpr xaml_point lerp_point(xaml_rectangle const& region, xaml_point const& p) noexcept
{
    return { region.x + region.width * p.x, region.y + region.height * p.y };
}

#endif // !XAML_UI_CANVAS_SHARED_POINT_HPP
