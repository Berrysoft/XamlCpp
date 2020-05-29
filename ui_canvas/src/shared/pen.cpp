#include <shared/pen.hpp>
#include <xaml/ui/controls/pen.h>

xaml_result XAML_CALL xaml_brush_pen_new(xaml_brush* brush, double width, xaml_brush_pen** ptr) noexcept
{
    return xaml_object_new<xaml_brush_pen_impl>(ptr, brush, width);
}
