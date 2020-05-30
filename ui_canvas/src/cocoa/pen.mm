#include <cocoa/drawing_mask.hpp>
#include <shared/pen.hpp>
#include <xaml/ui/cocoa/controls/brush.h>
#include <xaml/ui/controls/pen.h>

xaml_result xaml_brush_pen_impl::draw(NSBezierPath* path, xaml_size const& size, xaml_rectangle const& region) noexcept
{
    xaml_ptr<xaml_cocoa_brush> native_brush;
    XAML_RETURN_IF_FAILED(m_brush->query(&native_brush));
    return draw_mask(
        size,
        [this, path]() noexcept -> xaml_result {
            [path setLineWidth:m_width];
            [[NSColor whiteColor] set];
            [path stroke];
            return XAML_S_OK;
        },
        [this, native_brush, &size, &region]() noexcept -> xaml_result {
            xaml_rectangle new_region = region + xaml_margin{ m_width, m_width, m_width, m_width };
            NSBezierPath* regionPath = [NSBezierPath bezierPathWithRect:NSMakeRect(new_region.x, size.height - new_region.height - new_region.y, new_region.width, new_region.height)];
            return native_brush->draw(regionPath, size, region);
        });
}
