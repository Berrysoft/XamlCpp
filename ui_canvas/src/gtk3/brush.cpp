#include <cmath>
#include <gtk3/resources.hpp>
#include <shared/brush.hpp>
#include <shared/point.hpp>
#include <xaml/ui/controls/brush.h>

using namespace std;

xaml_result xaml_solid_brush_impl::set(cairo_t* handle, xaml_rectangle const&) noexcept
{
    cairo_set_source_rgba(handle, m_color.r / 255.0, m_color.g / 255.0, m_color.b / 255.0, m_color.a / 255.0);
    return XAML_S_OK;
}

static xaml_result add_stops(cairo_pattern_t* pattern, xaml_ptr<xaml_vector<xaml_gradient_stop>> const& stops) noexcept
{
    XAML_FOREACH_START(xaml_gradient_stop, item, stops);
    {
        cairo_pattern_add_color_stop_rgba(pattern, item.position, item.color.r / 255.0, item.color.g / 255.0, item.color.b / 255.0, item.color.a / 255.0);
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}

xaml_result xaml_linear_gradient_brush_impl::set(cairo_t* handle, xaml_rectangle const& region) noexcept
{
    xaml_point real_start_point = lerp_point(region, m_start_point);
    xaml_point real_end_point = lerp_point(region, m_end_point);
    unique_ptr<cairo_pattern_t, g_free_deleter<cairo_pattern_t, cairo_pattern_destroy>> pattern{
        cairo_pattern_create_linear(
            real_start_point.x, real_start_point.y,
            real_end_point.x, real_end_point.y)
    };
    XAML_RETURN_IF_FAILED(add_stops(pattern.get(), m_gradient_stops));
    cairo_set_source(handle, pattern.get());
    return XAML_S_OK;
}

xaml_result xaml_radial_gradient_brush_impl::set(cairo_t* handle, xaml_rectangle const& region) noexcept
{
    xaml_point real_origin = lerp_point(region, m_origin);
    xaml_point real_center = lerp_point(region, m_center);
    double rate = region.height / region.width * m_radius.height / m_radius.width;
    unique_ptr<cairo_pattern_t, g_free_deleter<cairo_pattern_t, cairo_pattern_destroy>> pattern{
        cairo_pattern_create_radial(
            real_origin.x, real_origin.y / rate, 0,
            real_center.x, real_center.y / rate, m_radius.width * region.width)
    };
    cairo_matrix_t matrix{};
    cairo_matrix_init_scale(&matrix, 1, 1 / rate);
    cairo_pattern_set_matrix(pattern.get(), &matrix);
    XAML_RETURN_IF_FAILED(add_stops(pattern.get(), m_gradient_stops));
    cairo_set_source(handle, pattern.get());
    return XAML_S_OK;
}
