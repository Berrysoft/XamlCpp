#import <internal/cocoa/global.h>
#include <xaml/ui/canvas.hpp>

using namespace std;

namespace xaml
{
    drawing_context::drawing_context(native_handle_type handle) : m_handle(handle)
    {
    }

    NSColor* get_NSColor(color c)
    {
        return [[NSColor colorWithCalibratedRed:c.r green:c.g blue:c.b alpha:c.a] autorelease];
    }

    void drawing_context::draw_ellipse(const drawing_pen& pen, const rectangle& region)
    {
        [NSGraphicsContext saveGraphicsState];
        [NSGraphicsContext setCurrentContext:m_handle];
        NSBezierPath* ellipse = [[NSBezierPath bezierPathWithOvalInRect:NSMakeRect(region.x, region.y, region.width, region.height)] autorelease];
        [ellipse setLineWidth:pen.get_width()];
        [get_NSColor(pen.get_color()) set];
        [ellipse stroke];
        [NSGraphicsContext restoreGraphicsState];
    }

    void drawing_context::draw_line(drawing_pen const& pen, point startp, point endp)
    {
        [NSGraphicsContext saveGraphicsState];
        [NSGraphicsContext setCurrentContext:m_handle];
        NSBezierPath* line = [[NSBezierPath bezierPath] autorelease];
        [line moveToPoint:NSMakePoint(startp.x, startp.y)];
        [line lineToPoint:NSMakePoint(endp.x, endp.y)];
        [line setLineWidth:pen.get_width()];
        [get_NSColor(pen.get_color()) set];
        [line stroke];
        [NSGraphicsContext restoreGraphicsState];
    }

    canvas::canvas() : common_control() {}

    canvas::~canvas() {}

    void canvas::__draw(const rectangle& region)
    {
        set_handle(get_parent()->get_handle());
        m_real_region = region - get_margin();
    }
}
