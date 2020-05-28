#include <shared/brush.hpp>
#include <vector>
#include <wil/com.h>

using namespace std;

xaml_result xaml_solid_brush_impl::create(ID2D1RenderTarget* target, ID2D1Brush** ptr) noexcept
{
    wil::com_ptr_t<ID2D1SolidColorBrush, wil::err_returncode_policy> brush;
    XAML_RETURN_IF_FAILED(target->CreateSolidColorBrush(D2D1::ColorF((uint32_t)m_color), &brush));
    return brush.copy_to(ptr);
}

xaml_result xaml_linear_gradient_brush_impl::create(ID2D1RenderTarget* target, ID2D1Brush** ptr) noexcept
{
    vector<D2D1_GRADIENT_STOP> stops;
    XAML_FOREACH_START(item, m_gradient_stops);
    {
        xaml_gradient_stop const* pitem;
        XAML_RETURN_IF_FAILED(item.query<xaml_box>()->get_value_ptr(&pitem));
        stops.push_back(D2D1::GradientStop((FLOAT)pitem->position, D2D1::ColorF((uint32_t)pitem->color)));
    }
    XAML_FOREACH_END();
    wil::com_ptr_t<ID2D1GradientStopCollection, wil::err_exception_policy> stop_collection;
    XAML_RETURN_IF_FAILED(target->CreateGradientStopCollection(stops.data(), (UINT32)stops.size(), &stop_collection));
    wil::com_ptr_t<ID2D1LinearGradientBrush, wil::err_returncode_policy> brush;
    XAML_RETURN_IF_FAILED(target->CreateLinearGradientBrush(
        D2D1::LinearGradientBrushProperties(
            D2D1::Point2F((FLOAT)m_start_point.x, (FLOAT)m_start_point.y),
            D2D1::Point2F((FLOAT)m_end_point.x, (FLOAT)m_end_point.y)),
        stop_collection.get(), &brush));
    return brush.copy_to(ptr);
}
