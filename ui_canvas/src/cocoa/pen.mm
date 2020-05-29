#include <shared/pen.hpp>
#include <xaml/ui/controls/pen.h>
#include <xaml/ui/drawing_conv.hpp>

xaml_result xaml_solid_pen_impl::set(NSBezierPath* path) noexcept
{
    [path setLineWidth:m_width];
    [xaml_to_native<NSColor*>(m_color) set];
    return XAML_S_OK;
}
