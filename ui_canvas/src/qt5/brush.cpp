#include <QBrush>
#include <shared/brush.hpp>
#include <shared/point.hpp>
#include <xaml/ui/drawing_conv.hpp>

using namespace std;

xaml_result xaml_solid_brush_impl::create(xaml_rectangle const&, QBrush* ptr) noexcept
{
    *ptr = QBrush{ QColor{ (uint32_t)m_color } };
    return XAML_S_OK;
}

static xaml_result set_colors(QGradient& gradient, xaml_ptr<xaml_vector> const& colors) noexcept
{
    XAML_FOREACH_START(item, colors);
    {
        xaml_gradient_stop stop;
        XAML_RETURN_IF_FAILED(xaml_unbox_value(item, &stop));
        gradient.setColorAt(stop.position, (uint32_t)stop.color);
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}

xaml_result xaml_linear_gradient_brush_impl::create(xaml_rectangle const& region, QBrush* ptr) noexcept
{
    QLinearGradient gradient{
        xaml_to_native<QPointF>(lerp_point(region, m_start_point)),
        xaml_to_native<QPointF>(lerp_point(region, m_end_point))
    };
    XAML_RETURN_IF_FAILED(set_colors(gradient, m_gradient_stops));
    *ptr = QBrush{ gradient };
    return XAML_S_OK;
}

xaml_result xaml_radial_gradient_brush_impl::create(xaml_rectangle const& region, QBrush* ptr) noexcept
{
    xaml_size real_radius = { m_radius.width * region.width, m_radius.height * region.height };
    QRadialGradient gradient{
        xaml_to_native<QPointF>(lerp_point(region, m_center)), real_radius.width,
        xaml_to_native<QPointF>(lerp_point(region, m_origin))
    };
    XAML_RETURN_IF_FAILED(set_colors(gradient, m_gradient_stops));
    QBrush brush{ gradient };
    brush.setTransform(QTransform::fromScale(1.0, real_radius.height / real_radius.width));
    *ptr = move(brush);
    return XAML_S_OK;
}
