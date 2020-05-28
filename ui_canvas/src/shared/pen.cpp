#include <shared/pen.hpp>
#include <xaml/ui/controls/pen.h>

xaml_result XAML_CALL xaml_solid_pen_new(xaml_color color, double width, xaml_solid_pen** ptr) noexcept
{
    return xaml_object_new<xaml_solid_pen_impl>(ptr, color, width);
}
