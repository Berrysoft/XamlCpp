#include <shared/pen.hpp>
#include <xaml/ui/controls/pen.h>

xaml_result XAML_CALL xaml_brush_pen_new(xaml_brush* brush, double width, xaml_brush_pen** ptr) noexcept
{
    return xaml_object_new<xaml_brush_pen_impl>(ptr, brush, width);
}

xaml_result XAML_CALL xaml_brush_pen_new_solid(xaml_color color, double width, xaml_brush_pen** ptr) noexcept
{
    xaml_ptr<xaml_solid_brush> brush;
    XAML_RETURN_IF_FAILED(xaml_solid_brush_new(color, &brush));
    return xaml_brush_pen_new(brush, width, ptr);
}
