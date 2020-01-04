#ifndef XAML_UI_INTERNAL_COCOA_DRAWING_HPP
#define XAML_UI_INTERNAL_COCOA_DRAWING_HPP

#import <Cocoa/Cocoa.h>
#include <xaml/ui/drawing.hpp>

namespace xaml
{
    constexpr rectangle get_rect(NSRect const& r) { return { r.origin.x, r.origin.y, r.size.width, r.size.height }; }

    constexpr NSPoint get_NSPoint(point p) { return { p.x, p.y }; }
    constexpr point get_point(NSPoint p) { return { p.x, p.y }; }
    constexpr NSSize get_NSSize(size s) { return { s.width, s.height }; }
    constexpr size get_size(NSSize s) { return { s.width, s.height }; }
} // namespace xaml

#endif // !XAML_UI_INTERNAL_COCOA_DRAWING_HPP
