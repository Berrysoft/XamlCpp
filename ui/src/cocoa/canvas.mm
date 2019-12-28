#include <functional>
#import <internal/cocoa/XamlCanvasView.h>
#include <xaml/ui/canvas.hpp>

@implementation XamlCanvasView : NSView
@synthesize classPointer;

- (id)initWithClassPointer:(void*)ptr
{
    if ([self init])
    {
        self.classPointer = ptr;
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
    xaml::canvas* cv = (xaml::canvas*)self.classPointer;
    cv->__on_draw_rect();
}
@end

using namespace std;

namespace xaml
{
    drawing_context::drawing_context(native_handle_type handle) : m_handle(handle)
    {
    }

    static NSColor* get_NSColor(color c)
    {
        return [NSColor colorWithCalibratedRed:(c.r / 255.0) green:(c.g / 255.0) blue:(c.b / 255.0) alpha:(c.a / 255.0)];
    }

    static void set_pen(NSBezierPath* path, drawing_pen const& pen)
    {
        [path setLineWidth:pen.get_width()];
        [get_NSColor(pen.get_color()) set];
    }

    static void set_brush(NSBezierPath* path, drawing_brush const& brush)
    {
        [get_NSColor(brush.get_color()) set];
    }

    NSBezierPath* drawing_context::path_ellipse(rectangle const& region)
    {
        NSLog(@"y:%lf, width:%lf", m_size.height - region.height - region.y, region.width);
        return [NSBezierPath bezierPathWithOvalInRect:NSMakeRect(region.x, m_size.height - region.height - region.y, region.width, region.height)];
    }

    void drawing_context::draw_ellipse(drawing_pen const& pen, rectangle const& region)
    {
        NSBezierPath* ellipse = path_ellipse(region);
        set_pen(ellipse, pen);
        [ellipse stroke];
    }

    void drawing_context::fill_ellipse(drawing_brush const& brush, rectangle const& region)
    {
        NSBezierPath* ellipse = path_ellipse(region);
        set_brush(ellipse, brush);
        [ellipse fill];
    }

    void drawing_context::draw_line(drawing_pen const& pen, point startp, point endp)
    {
        NSBezierPath* line = [NSBezierPath bezierPath];
        [line moveToPoint:NSMakePoint(startp.x, startp.y)];
        [line lineToPoint:NSMakePoint(endp.x, endp.y)];
        set_pen(line, pen);
        [line stroke];
    }

    NSBezierPath* drawing_context::path_rect(rectangle const& rect)
    {
        return [NSBezierPath bezierPathWithRect:NSMakeRect(rect.x, m_size.height - rect.height - rect.y, rect.width, rect.height)];
    }

    void drawing_context::draw_rect(drawing_pen const& pen, rectangle const& rect)
    {
        NSBezierPath* path = path_rect(rect);
        set_pen(path, pen);
        [path stroke];
    }

    void drawing_context::fill_rect(drawing_brush const& brush, rectangle const& rect)
    {
        NSBezierPath* path = path_rect(rect);
        set_brush(path, brush);
        [path fill];
    }

    NSBezierPath* drawing_context::path_round_rect(rectangle const& rect, size round)
    {
        return [NSBezierPath bezierPathWithRoundedRect:NSMakeRect(rect.x, m_size.height - rect.height - rect.y, rect.width, rect.height)
                                               xRadius:round.width / 2
                                               yRadius:round.height / 2];
    }

    void drawing_context::draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round)
    {
        NSBezierPath* path = path_round_rect(rect, round);
        set_pen(path, pen);
        [path stroke];
    }

    void drawing_context::fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round)
    {
        NSBezierPath* path = path_round_rect(rect, round);
        set_brush(path, brush);
        [path fill];
    }

    canvas::canvas() : common_control()
    {
    }

    canvas::~canvas() {}

    void canvas::__draw(const rectangle& region)
    {
        if (!get_handle())
        {
            XamlCanvasView* view = [[XamlCanvasView alloc] initWithClassPointer:this];
            set_handle(view);
        }
        m_real_region = region - get_margin();
        XamlCanvasView* view = (XamlCanvasView*)get_handle();
        NSWindow* window = (NSWindow*)get_parent()->get_handle();
        NSRect frame = window.contentView.frame;
        view.frame = NSMakeRect(m_real_region.x, frame.size.height - m_real_region.height - m_real_region.y, m_real_region.width, m_real_region.height);
        [view setNeedsDisplay:YES];
    }

    void canvas::__on_draw_rect()
    {
        drawing_context dc{ nullptr };
        dc.set_size({ m_real_region.width, m_real_region.height });
        m_redraw(*this, dc);
    }
}
