#include <shared/pen.hpp>
#include <xaml/ui/cocoa/controls/brush.h>
#include <xaml/ui/controls/pen.h>

xaml_result xaml_brush_pen_impl::draw(NSBezierPath* path, xaml_size const& size, xaml_rectangle const& region) noexcept
{
    xaml_ptr<xaml_cocoa_brush> native_brush;
    XAML_RETURN_IF_FAILED(m_brush->query(&native_brush));
    CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceGray();
    CGContextRef maskContext = CGBitmapContextCreate(nullptr, size.width, size.height, 8, size.width, colorspace, 0);
    CGColorSpaceRelease(colorspace);

    NSGraphicsContext* maskGraphicsContext = [NSGraphicsContext graphicsContextWithCGContext:maskContext
                                                                                     flipped:NO];
    [NSGraphicsContext saveGraphicsState];
    [NSGraphicsContext setCurrentContext:maskGraphicsContext];

    [path setLineWidth:m_width];
    [[NSColor whiteColor] set];
    [path stroke];

    [NSGraphicsContext restoreGraphicsState];

    CGImageRef alphaMask = CGBitmapContextCreateImage(maskContext);

    CGContextRef windowContext = [[NSGraphicsContext currentContext] CGContext];
    CGContextSaveGState(windowContext);
    CGContextClipToMask(windowContext, { 0, 0, size.width, size.height }, alphaMask);

    xaml_rectangle new_region = region + xaml_margin{ m_width, m_width, m_width, m_width };
    NSBezierPath* regionPath = [NSBezierPath bezierPathWithRect:NSMakeRect(new_region.x, size.height - new_region.height - new_region.y, new_region.width, new_region.height)];
    xaml_result hr = native_brush->draw(regionPath, size, region);

    CGContextRestoreGState(windowContext);
    CGImageRelease(alphaMask);
    return hr;
}
