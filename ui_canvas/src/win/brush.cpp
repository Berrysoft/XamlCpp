#include <shared/brush.hpp>
#include <shared/point.hpp>
#include <vector>
#include <wil/com.h>
#include <win/d2d_conv.hpp>

using namespace std;

xaml_result xaml_solid_brush_impl::create(ID2D1RenderTarget* target, xaml_rectangle const&, ID2D1Brush** ptr) noexcept
{
    wil::com_ptr_nothrow<ID2D1SolidColorBrush> brush;
    XAML_RETURN_IF_FAILED(target->CreateSolidColorBrush(D2D1::ColorF((uint32_t)m_color), &brush));
    return brush.copy_to(ptr);
}

static xaml_result get_STOPs(ID2D1RenderTarget* target, xaml_ptr<xaml_vector<xaml_gradient_stop>> const& vec, ID2D1GradientStopCollection** ptr) noexcept
{
    vector<D2D1_GRADIENT_STOP> stops;
    XAML_FOREACH_START(xaml_gradient_stop, item, vec);
    {
        stops.push_back(D2D1::GradientStop((FLOAT)item.position, D2D1::ColorF((uint32_t)item.color)));
    }
    XAML_FOREACH_END();
    return target->CreateGradientStopCollection(stops.data(), (UINT32)stops.size(), ptr);
}

xaml_result xaml_linear_gradient_brush_impl::create(ID2D1RenderTarget* target, xaml_rectangle const& region, ID2D1Brush** ptr) noexcept
{
    wil::com_ptr_t<ID2D1GradientStopCollection, wil::err_exception_policy> stop_collection;
    XAML_RETURN_IF_FAILED(get_STOPs(target, m_gradient_stops, &stop_collection));
    wil::com_ptr_nothrow<ID2D1LinearGradientBrush> brush;
    XAML_RETURN_IF_FAILED(target->CreateLinearGradientBrush(
        D2D1::LinearGradientBrushProperties(
            xaml_to_native<D2D1_POINT_2F>(lerp_point(region, m_start_point)),
            xaml_to_native<D2D1_POINT_2F>(lerp_point(region, m_end_point))),
        stop_collection.get(), &brush));
    return brush.copy_to(ptr);
}

xaml_result xaml_radial_gradient_brush_impl::create(ID2D1RenderTarget* target, xaml_rectangle const& region, ID2D1Brush** ptr) noexcept
{
    wil::com_ptr_t<ID2D1GradientStopCollection, wil::err_exception_policy> stop_collection;
    XAML_RETURN_IF_FAILED(get_STOPs(target, m_gradient_stops, &stop_collection));
    wil::com_ptr_nothrow<ID2D1RadialGradientBrush> brush;
    XAML_RETURN_IF_FAILED(target->CreateRadialGradientBrush(
        D2D1::RadialGradientBrushProperties(
            xaml_to_native<D2D1_POINT_2F>(lerp_point(region, m_center)),
            D2D1::Point2F((FLOAT)(region.width * (m_origin - m_center).x), (FLOAT)(region.height * (m_origin - m_center).y)),
            (FLOAT)(m_radius.width * region.width), (FLOAT)(m_radius.height * region.height)),
        stop_collection.get(), &brush));
    return brush.copy_to(ptr);
}
