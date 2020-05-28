#include <shared/brush.hpp>
#include <xaml/ui/controls/brush.h>

xaml_result xaml_solid_brush_impl::set(NSBezierPath* path) noexcept
{
    [[NSColor colorWithCalibratedRed:(m_color.r / 255.0) green:(m_color.g / 255.0) blue:(m_color.b / 255.0) alpha:(m_color.a / 255.0)] set];
    return XAML_S_OK;
}
