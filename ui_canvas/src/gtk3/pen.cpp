#include <shared/pen.hpp>
#include <xaml/ui/controls/pen.h>

xaml_result xaml_solid_pen_impl::set(cairo_t* handle) noexcept
{
    cairo_set_source_rgba(handle, m_color.r / 255.0, m_color.g / 255.0, m_color.b / 255.0, m_color.a / 255.0);
    cairo_set_line_width(handle, m_width);
    return XAML_S_OK;
}
