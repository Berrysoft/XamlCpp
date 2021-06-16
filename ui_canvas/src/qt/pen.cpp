#include <QBrush>
#include <QPen>
#include <shared/pen.hpp>
#include <xaml/ui/qt5/controls/brush.hpp>

xaml_result xaml_brush_pen_impl::create(xaml_rectangle const& region, QPen* ptr) noexcept
{
    xaml_ptr<xaml_qt5_brush> native_brush;
    XAML_RETURN_IF_FAILED(m_brush.query(&native_brush));
    QBrush brush;
    XAML_RETURN_IF_FAILED(native_brush->create(region, &brush));
    *ptr = QPen{ brush, m_width };
    return XAML_S_OK;
}
