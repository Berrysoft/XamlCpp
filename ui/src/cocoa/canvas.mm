#include <functional>
#include <xaml/ui/canvas.hpp>

using namespace std;

namespace xaml
{
    drawing_context::drawing_context(native_handle_type handle) : m_handle(handle)
    {
    }

    static NSColor* get_NSColor(color c)
    {
        return [NSColor colorWithCalibratedRed:c.r green:c.g blue:c.b alpha:c.a];
    }

    static void draw_ns_graphics(NSGraphicsContext* graphics, function<void()> func)
    {
        [NSGraphicsContext saveGraphicsState];
        [NSGraphicsContext setCurrentContext:graphics];
        func();
        [NSGraphicsContext restoreGraphicsState];
    }

    static void set_pen(NSBezierPath* path, drawing_pen const& pen)
    {
        [path setLineWidth:pen.get_width()];
        [get_NSColor(pen.get_color()) setStroke];
    }

    static void set_brush(NSBezierPath* path, drawing_brush const& brush)
    {
        [get_NSColor(brush.get_color()) setFill];
    }

    NSBezierPath* drawing_context::path_ellipse(rectangle const& region)
    {
        return [NSBezierPath bezierPathWithOvalInRect:NSMakeRect(region.x, m_size.height - region.height - region.y, region.width, region.height)];
    }

    void drawing_context::draw_ellipse(drawing_pen const& pen, rectangle const& region)
    {
        draw_ns_graphics(m_handle, [&]() -> void {
            NSBezierPath* ellipse = path_ellipse(region);
            set_pen(ellipse, pen);
            [ellipse stroke];
        });
    }

    void drawing_context::fill_ellipse(drawing_brush const& brush, rectangle const& region)
    {
        draw_ns_graphics(m_handle, [&]() -> void {
            NSBezierPath* ellipse = path_ellipse(region);
            set_brush(ellipse, brush);
            [ellipse fill];
        });
    }

    void drawing_context::draw_line(drawing_pen const& pen, point startp, point endp)
    {
        draw_ns_graphics(m_handle, [&]() -> void {
            NSBezierPath* line = [NSBezierPath bezierPath];
            [line moveToPoint:NSMakePoint(startp.x, startp.y)];
            [line lineToPoint:NSMakePoint(endp.x, endp.y)];
            set_pen(line, pen);
            [line stroke];
        });
    }

    NSBezierPath* drawing_context::path_rect(rectangle const& rect)
    {
        return [NSBezierPath bezierPathWithRect:NSMakeRect(rect.x, m_size.height - rect.height - rect.y, rect.width, rect.height)];
    }

    void drawing_context::draw_rect(drawing_pen const& pen, rectangle const& rect)
    {
        draw_ns_graphics(m_handle, [&]() -> void {
            NSBezierPath* path = path_rect(rect);
            set_pen(path, pen);
            [path stroke];
        });
    }

    void drawing_context::fill_rect(drawing_brush const& brush, rectangle const& rect)
    {
        draw_ns_graphics(m_handle, [&]() -> void {
            NSBezierPath* path = path_rect(rect);
            set_brush(path, brush);
            [path fill];
        });
    }

    NSBezierPath* drawing_context::path_round_rect(rectangle const& rect, size round)
    {
        return [NSBezierPath bezierPathWithRoundedRect:NSMakeRect(rect.x, m_size.height - rect.height - rect.y, rect.width, rect.height)
                                               xRadius:round.width / 2
                                               yRadius:round.height / 2];
    }

    void drawing_context::draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round)
    {
        draw_ns_graphics(m_handle, [&]() -> void {
            NSBezierPath* path = path_round_rect(rect, round);
            set_pen(path, pen);
            [path stroke];
        });
    }

    void drawing_context::fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round)
    {
        draw_ns_graphics(m_handle, [&]() -> void {
            NSBezierPath* path = path_round_rect(rect, round);
            set_brush(path, brush);
            [path fill];
        });
    }

    canvas::canvas() : common_control()
    {
    }

    canvas::~canvas() {}

    void canvas::__draw(const rectangle& region)
    {
        set_handle(get_parent()->get_handle());
        m_real_region = region - get_margin();
    }
}
