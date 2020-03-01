#import <cocoa/XamlCanvasView.h>
#include <functional>
#include <xaml/ui/controls/canvas.hpp>

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
    static NSColor* get_NSColor(color c)
    {
        return [NSColor colorWithCalibratedRed:(c.r / 255.0) green:(c.g / 255.0) blue:(c.b / 255.0) alpha:(c.a / 255.0)];
    }

    static void set_pen(NSBezierPath* path, drawing_pen const& pen)
    {
        [path setLineWidth:pen.width];
        [get_NSColor(pen.stroke) set];
    }

    static void set_brush(NSBezierPath* path, drawing_brush const& brush)
    {
        [get_NSColor(brush.fill) set];
    }

    static NSBezierPath* path_ellipse(size base_size, rectangle const& region)
    {
        return [NSBezierPath bezierPathWithOvalInRect:NSMakeRect(region.x, base_size.height - region.height - region.y, region.width, region.height)];
    }

    void drawing_context::draw_ellipse(drawing_pen const& pen, rectangle const& region)
    {
        NSBezierPath* ellipse = path_ellipse(m_size, region);
        set_pen(ellipse, pen);
        [ellipse stroke];
    }

    void drawing_context::fill_ellipse(drawing_brush const& brush, rectangle const& region)
    {
        NSBezierPath* ellipse = path_ellipse(m_size, region);
        set_brush(ellipse, brush);
        [ellipse fill];
    }

    void drawing_context::draw_line(drawing_pen const& pen, point startp, point endp)
    {
        NSBezierPath* line = [NSBezierPath bezierPath];
        [line moveToPoint:NSMakePoint(startp.x, m_size.height - startp.y)];
        [line lineToPoint:NSMakePoint(endp.x, m_size.height - endp.y)];
        set_pen(line, pen);
        [line stroke];
    }

    static NSBezierPath* path_rect(size base_size, rectangle const& rect)
    {
        return [NSBezierPath bezierPathWithRect:NSMakeRect(rect.x, base_size.height - rect.height - rect.y, rect.width, rect.height)];
    }

    void drawing_context::draw_rect(drawing_pen const& pen, rectangle const& rect)
    {
        NSBezierPath* path = path_rect(m_size, rect);
        set_pen(path, pen);
        [path stroke];
    }

    void drawing_context::fill_rect(drawing_brush const& brush, rectangle const& rect)
    {
        NSBezierPath* path = path_rect(m_size, rect);
        set_brush(path, brush);
        [path fill];
    }

    static NSBezierPath* path_round_rect(size base_size, rectangle const& rect, size round)
    {
        return [NSBezierPath bezierPathWithRoundedRect:NSMakeRect(rect.x, base_size.height - rect.height - rect.y, rect.width, rect.height)
                                               xRadius:round.width / 2
                                               yRadius:round.height / 2];
    }

    void drawing_context::draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round)
    {
        NSBezierPath* path = path_round_rect(m_size, rect, round);
        set_pen(path, pen);
        [path stroke];
    }

    void drawing_context::fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round)
    {
        NSBezierPath* path = path_round_rect(m_size, rect, round);
        set_brush(path, brush);
        [path fill];
    }

    void drawing_context::draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str)
    {
        NSFontDescriptor* fontdes = [NSFontDescriptor fontDescriptorWithName:[NSString stringWithUTF8String:font.font_family.c_str()]
                                                                        size:font.size];
        NSFontDescriptorSymbolicTraits traits = 0;
        if (font.italic) traits |= NSFontDescriptorTraitItalic;
        if (font.bold) traits |= NSFontDescriptorTraitBold;
        if (traits)
        {
            fontdes = [fontdes fontDescriptorWithSymbolicTraits:traits];
        }
        NSFont* nfont = [NSFont fontWithDescriptor:fontdes size:font.size];
        NSDictionary* attrs = @{
            NSFontAttributeName : nfont,
            NSForegroundColorAttributeName : get_NSColor(brush.fill)
        };
        NSAttributedString* astr = [[NSAttributedString alloc] initWithString:[NSString stringWithUTF8String:str.data()]
                                                                   attributes:attrs];
        [astr drawAtPoint:to_native<NSPoint>(p)];
    }

    void canvas::__draw(const rectangle& region)
    {
        if (!get_handle())
        {
            XamlCanvasView* view = [[XamlCanvasView alloc] initWithClassPointer:this];
            set_handle(view);
        }
        m_real_region = region - get_margin();
        XamlCanvasView* view = (XamlCanvasView*)get_handle();
        NSView* pview = get_parent().lock()->get_handle();
        NSRect frame = pview.frame;
        view.frame = NSMakeRect(m_real_region.x, frame.size.height - m_real_region.height - m_real_region.y, m_real_region.width, m_real_region.height);
        [view setNeedsDisplay:YES];
    }

    void canvas::__on_draw_rect()
    {
        drawing_context dc{ nullptr };
        dc.__set_size({ m_real_region.width, m_real_region.height });
        m_redraw(*this, dc);
    }
}
