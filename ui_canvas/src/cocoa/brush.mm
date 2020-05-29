#include <shared/brush.hpp>
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

xaml_result xaml_linear_gradient_brush_impl::draw(NSBezierPath* path, xaml_size const& size, xaml_rectangle const& region) noexcept
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
    xaml_point dir = m_end_point - m_start_point;
    [gradient drawInBezierPath:path angle:-atan(dir.y / dir.x) / pi * 180.0];
    return XAML_S_OK;
}
