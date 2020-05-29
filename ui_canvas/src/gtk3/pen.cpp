#include <shared/pen.hpp>
#include <xaml/ui/controls/pen.h>
#include <xaml/ui/gtk3/controls/brush.h>

xaml_result xaml_brush_pen_impl::set(cairo_t* handle, xaml_rectangle const& region) noexcept
{
    xaml_ptr<xaml_gtk3_brush> native_brush;
    XAML_RETURN_IF_FAILED(m_brush->query(&native_brush));
    XAML_RETURN_IF_FAILED(native_brush->set(handle, region));
    cairo_set_line_width(handle, m_width);
    return XAML_S_OK;
}
