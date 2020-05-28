#include <shared/brush.hpp>
#include <xaml/ui/controls/brush.h>

using namespace std;

xaml_result XAML_CALL xaml_solid_brush_new(xaml_color color, xaml_solid_brush** ptr) noexcept
{
    return xaml_object_new<xaml_solid_brush_impl>(ptr, color);
}
