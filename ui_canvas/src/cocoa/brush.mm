#include <cmath>
#include <cocoa/drawing_mask.hpp>
#include <shared/brush.hpp>
#include <shared/point.hpp>
#include <vector>
#include <xaml/ui/controls/brush.h>
#include <xaml/ui/drawing_conv.hpp>

#if __has_include(<numbers>)
    #include <numbers>
using std::numbers::pi;
#else
    #define pi M_PI
#endif // __has_include(<numbers>)

using namespace std;

xaml_result xaml_solid_brush_impl::draw(NSBezierPath* path, xaml_size const&, xaml_rectangle const&) noexcept
{
    [xaml_to_native<NSColor*>(m_color) set];
    [path fill];
    return XAML_S_OK;
}

static xaml_result get_Gradient(xaml_ptr<xaml_vector<xaml_gradient_stop>> const& stops, NSGradient** pres) noexcept
{
    NSMutableArray<NSColor*>* colors = [NSMutableArray<NSColor*> new];
    vector<CGFloat> locations;
    XAML_FOREACH_START(xaml_gradient_stop, item, stops);
    {
        [colors addObject:xaml_to_native<NSColor*>(item.color)];
        locations.push_back(item.position);
    }
    XAML_FOREACH_END();
    NSGradient* gradient = [[NSGradient alloc] initWithColors:colors
                                                  atLocations:locations.data()
                                                   colorSpace:[NSColorSpace deviceRGBColorSpace]];
    *pres = gradient;
    return XAML_S_OK;
}

xaml_result xaml_linear_gradient_brush_impl::draw(NSBezierPath* path, xaml_size const& size, xaml_rectangle const& region) noexcept
{
    NSGradient* gradient;
    XAML_RETURN_IF_FAILED(get_Gradient(m_gradient_stops, &gradient));
    xaml_point dir = m_end_point - m_start_point;
    [gradient drawInBezierPath:path angle:-atan(dir.y / dir.x) / pi * 180.0];
    return XAML_S_OK;
}

xaml_result xaml_radial_gradient_brush_impl::draw(NSBezierPath* path, xaml_size const& size, xaml_rectangle const& region) noexcept
{
    NSGradient* gradient;
    XAML_RETURN_IF_FAILED(get_Gradient(m_gradient_stops, &gradient));
    return draw_mask(
        size,
        [this, path]() noexcept -> xaml_result {
            [[NSColor whiteColor] set];
            [path fill];
            return XAML_S_OK;
        },
        [this, gradient, &size, &region]() noexcept -> xaml_result {
            xaml_point real_origin = lerp_point(region, m_origin);
            xaml_point real_center = lerp_point(region, m_center);
            double rate = region.height / region.width * m_radius.height / m_radius.width;
            NSAffineTransform* transform = [NSAffineTransform transform];
            [transform scaleXBy:1 yBy:rate];
            [transform concat];
            [gradient drawFromCenter:NSMakePoint(real_origin.x, (size.height - real_origin.y) / rate)
                              radius:0
                            toCenter:NSMakePoint(real_center.x, (size.height - real_center.y) / rate)
                              radius:sqrt(region.width * region.width + region.height * region.height) * m_radius.width
                             options:NSGradientDrawsAfterEndingLocation];
            return XAML_S_OK;
        });
}
