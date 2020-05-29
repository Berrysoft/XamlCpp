#include <shared/brush.hpp>
#include <vector>
#include <xaml/ui/controls/brush.h>
#include <xaml/ui/drawing_conv.hpp>

using namespace std;

xaml_result xaml_solid_brush_impl::set(NSBezierPath* path, xaml_size const&, xaml_rectangle const&) noexcept
{
    [xaml_to_native<NSColor*>(m_color) set];
    return XAML_S_OK;
}

xaml_result xaml_linear_gradient_brush_impl::set(NSBezierPath* path, xaml_size const& size, xaml_rectangle const& region) noexcept
{
    NSMutableArray<NSColor*>* colors = [NSMutableArray<NSColor*> new];
    vector<CGFloat> locations;
    XAML_FOREACH_START(item, m_gradient_stops);
    {
        xaml_gradient_stop const* pitem;
        XAML_RETURN_IF_FAILED(item.query<xaml_box>()->get_value_ptr(&pitem));
        [colors addObject:xaml_to_native<NSColor*>(pitem->color)];
        locations.push_back(pitem->position);
    }
    XAML_FOREACH_END();
    NSGradient* gradient = [[NSGradient alloc] initWithColors:colors
                                                  atLocations:locations.data()
                                                   colorSpace:[NSColorSpace deviceRGBColorSpace]];
    [gradient drawFromPoint:NSMakePoint(lerp(region.x, region.x + region.width, m_start_point.x), size.height - region.height - lerp(region.y, region.y + region.height, m_start_point.y))
                    toPoint:NSMakePoint(lerp(region.x, region.x + region.width, m_end_point.x), size.height - region.height - lerp(region.y, region.y + region.height, m_end_point.y))
                    options:NSGradientDrawsAfterEndingLocation];
    return XAML_S_OK;
}
