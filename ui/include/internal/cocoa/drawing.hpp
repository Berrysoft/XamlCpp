#ifndef XAML_UI_INTERNAL_COCOA_DRAWING_HPP
#define XAML_UI_INTERNAL_COCOA_DRAWING_HPP

#import <Cocoa/Cocoa.h>
#include <xaml/ui/drawing.hpp>

namespace xaml
{
    constexpr rectangle get_rect(NSRect const& r) { return { r.origin.x, r.origin.y, r.size.width, r.size.height }; }
} // namespace xaml

#endif // !XAML_UI_INTERNAL_COCOA_DRAWING_HPP
