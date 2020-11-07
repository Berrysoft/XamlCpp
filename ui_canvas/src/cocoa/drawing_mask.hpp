#ifndef XAML_UI_CANVAS_COCOA_DRAWING_MASK_HPP
#define XAML_UI_CANVAS_COCOA_DRAWING_MASK_HPP

#include <function2/function2.hpp>
#include <xaml/ui/cocoa/objc.h>
#include <xaml/ui/controls/canvas.h>

inline xaml_result XAML_CALL draw_mask(xaml_size const& size, fu2::unique_function<xaml_result() noexcept> mask, fu2::unique_function<xaml_result() noexcept> fill) noexcept
{
    xaml_result hr;

    CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceGray();
    CGContextRef maskContext = CGBitmapContextCreate(nullptr, size.width, size.height, 8, size.width, colorspace, 0);
    CGColorSpaceRelease(colorspace);

    NSGraphicsContext* maskGraphicsContext = [NSGraphicsContext graphicsContextWithCGContext:maskContext
                                                                                     flipped:NO];
    [NSGraphicsContext saveGraphicsState];
    [NSGraphicsContext setCurrentContext:maskGraphicsContext];

    hr = mask();

    [NSGraphicsContext restoreGraphicsState];

    if (XAML_FAILED(hr)) return hr;

    CGImageRef alphaMask = CGBitmapContextCreateImage(maskContext);

    CGContextRef windowContext = [[NSGraphicsContext currentContext] CGContext];
    CGContextSaveGState(windowContext);
    CGContextClipToMask(windowContext, { 0, 0, size.width, size.height }, alphaMask);

    hr = fill();

    CGContextRestoreGState(windowContext);
    CGImageRelease(alphaMask);
    return hr;
}

#endif // !XAML_UI_CANVAS_COCOA_DRAWING_MASK_HPP
