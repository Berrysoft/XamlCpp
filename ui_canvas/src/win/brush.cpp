#include <shared/brush.hpp>
#include <vector>
#include <wil/com.h>

using namespace std;

xaml_result xaml_solid_brush_impl::create(ID2D1RenderTarget* target, xaml_rectangle const&, ID2D1Brush** ptr) noexcept
{
    wil::com_ptr_t<ID2D1SolidColorBrush, wil::err_returncode_policy> brush;
    XAML_RETURN_IF_FAILED(target->CreateSolidColorBrush(D2D1::ColorF((uint32_t)m_color), &brush));
    return brush.copy_to(ptr);
}

static xaml_result get_STOPs(ID2D1RenderTarget* target, xaml_ptr<xaml_vector> const& vec, ID2D1GradientStopCollection** ptr) noexcept
{
    vector<D2D1_GRADIENT_STOP> stops;
    XAML_FOREACH_START(item, vec);
    {
        xaml_gradient_stop const* pitem;
        XAML_RETURN_IF_FAILED(item.query<xaml_box>()->get_value_ptr(&pitem));
        stops.push_back(D2D1::GradientStop((FLOAT)pitem->position, D2D1::ColorF((uint32_t)pitem->color)));
    }
    XAML_FOREACH_END();
    return target->CreateGradientStopCollection(stops.data(), (UINT32)stops.size(), ptr);
}

xaml_result xaml_linear_gradient_brush_impl::create(ID2D1RenderTarget* target, xaml_rectangle const& region, ID2D1Brush** ptr) noexcept
{
    wil::com_ptr_t<ID2D1GradientStopCollection, wil::err_exception_policy> stop_collection;
    XAML_RETURN_IF_FAILED(get_STOPs(target, m_gradient_stops, &stop_collection));
    wil::com_ptr_t<ID2D1LinearGradientBrush, wil::err_returncode_policy> brush;
    XAML_RETURN_IF_FAILED(target->CreateLinearGradientBrush(
        D2D1::LinearGradientBrushProperties(
            D2D1::Point2F((FLOAT)lerp(region.x, region.x + region.width, m_start_point.x), (FLOAT)lerp(region.y, region.y + region.height, m_start_point.y)),
            D2D1::Point2F((FLOAT)lerp(region.x, region.x + region.width, m_end_point.x), (FLOAT)lerp(region.y, region.y + region.height, m_end_point.y))),
        stop_collection.get(), &brush));
    return brush.copy_to(ptr);
}

xaml_result xaml_radial_gradient_brush_impl::create(ID2D1RenderTarget* target, xaml_rectangle const& region, ID2D1Brush** ptr) noexcept
{
    wil::com_ptr_t<ID2D1GradientStopCollection, wil::err_exception_policy> stop_collection;
    XAML_RETURN_IF_FAILED(get_STOPs(target, m_gradient_stops, &stop_collection));
    wil::com_ptr_t<ID2D1RadialGradientBrush, wil::err_returncode_policy> brush;
    XAML_RETURN_IF_FAILED(target->CreateRadialGradientBrush(
        D2D1::RadialGradientBrushProperties(
            D2D1::Point2F((FLOAT)lerp(region.x, region.x + region.width, m_center.x), (FLOAT)lerp(region.y, region.y + region.height, m_center.y)),
            D2D1::Point2F((FLOAT)(region.width * (m_origin - m_center).x), (FLOAT)(region.height * (m_origin - m_center).y)),
            (FLOAT)(m_radius.width * region.width), (FLOAT)(m_radius.height * region.height)),
        stop_collection.get(), &brush));
    return brush.copy_to(ptr);
}
