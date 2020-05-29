#include <cmath>
#include <gtk3/resources.hpp>
#include <shared/brush.hpp>
#include <xaml/ui/controls/brush.h>

using namespace std;

xaml_result xaml_solid_brush_impl::set(cairo_t* handle, xaml_rectangle const&) noexcept
{
    cairo_set_source_rgba(handle, m_color.r / 255.0, m_color.g / 255.0, m_color.b / 255.0, m_color.a / 255.0);
    return XAML_S_OK;
}

static xaml_result add_stops(cairo_pattern_t* pattern, xaml_ptr<xaml_vector> const& stops) noexcept
{
    XAML_FOREACH_START(item, stops);
    {
        xaml_gradient_stop const* pitem;
        XAML_RETURN_IF_FAILED(item.query<xaml_box>()->get_value_ptr(&pitem));
        cairo_pattern_add_color_stop_rgba(pattern, pitem->position, pitem->color.r / 255.0, pitem->color.g / 255.0, pitem->color.b / 255.0, pitem->color.a / 255.0);
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}

xaml_result xaml_linear_gradient_brush_impl::set(cairo_t* handle, xaml_rectangle const& region) noexcept
{
    unique_ptr<cairo_pattern_t, g_free_deleter<cairo_pattern_t, cairo_pattern_destroy>> pattern{
        cairo_pattern_create_linear(
            lerp(region.x, region.x + region.width, m_start_point.x),
            lerp(region.y, region.y + region.height, m_start_point.y),
            lerp(region.x, region.x + region.width, m_end_point.x),
            lerp(region.y, region.y + region.height, m_end_point.y))
    };
    add_stops(pattern.get(), m_gradient_stops);
    cairo_set_source(handle, pattern.get());
    return XAML_S_OK;
}

xaml_result xaml_radial_gradient_brush_impl::set(cairo_t* handle, xaml_rectangle const& region) noexcept
{
    unique_ptr<cairo_pattern_t, g_free_deleter<cairo_pattern_t, cairo_pattern_destroy>> pattern{
        cairo_pattern_create_radial(
            lerp(region.x, region.x + region.width, m_origin.x),
            lerp(region.y, region.y + region.height, m_origin.y),
            0,
            lerp(region.x, region.x + region.width, m_center.x),
            lerp(region.y, region.y + region.height, m_center.y),
            m_radius.width * region.width)
    };
    cairo_matrix_t matrix;
    cairo_matrix_init_scale(&matrix, 1, m_radius.height / m_radius.width);
    cairo_pattern_set_matrix(pattern.get(), &matrix);
    add_stops(pattern.get(), m_gradient_stops);
    cairo_set_source(handle, pattern.get());
    return XAML_S_OK;
}
