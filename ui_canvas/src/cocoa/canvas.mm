#include <cmath>
#import <cocoa/XamlCanvasView.h>
#include <cocoa/nsstring.hpp>
#include <functional>
#include <shared/canvas.hpp>
#include <xaml/ui/controls/canvas.h>
#include <xaml/ui/native_drawing.hpp>

@implementation XamlCanvasView : NSView
@synthesize classPointer;

- (instancetype)initWithClassPointer:(void*)ptr
{
    if ([self init])
    {
        self.classPointer = ptr;
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
    xaml_canvas_internal* cv = (xaml_canvas_internal*)self.classPointer;
    cv->on_draw_rect();
}
@end

#if __has_include(<numbers>)
#include <numbers>
using std::numbers::pi;
#else
inline constexpr double pi = M_PI;
#endif // __has_include(<numbers>)

using namespace std;

static NSColor* get_NSColor(xaml_color c) noexcept
{
    return [NSColor colorWithCalibratedRed:(c.r / 255.0) green:(c.g / 255.0) blue:(c.b / 255.0) alpha:(c.a / 255.0)];
}

static void set_pen(NSBezierPath* path, xaml_drawing_pen const& pen) noexcept
{
    [path setLineWidth:pen.width];
    [get_NSColor(pen.stroke) set];
}

static void set_brush(NSBezierPath* path, xaml_drawing_brush const& brush) noexcept
{
    [get_NSColor(brush.fill) set];
}

static NSBezierPath* path_arc(xaml_size const& base_size, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    NSBezierPath* path = [NSBezierPath bezierPath];
    xaml_size radius = { region.width / 2, region.height / 2 };
    xaml_point centerp = { region.x + radius.width, region.y + radius.height };
    xaml_point startp = centerp + xaml_point{ radius.width * cos(start_angle), radius.height * sin(start_angle) };
    [path moveToPoint:NSMakePoint(startp.x, base_size.height - startp.y)];
    [path appendBezierPathWithArcWithCenter:NSMakePoint(centerp.x, base_size.height - centerp.y)
                                     radius:radius.width
                                 startAngle:-start_angle / pi * 180
                                   endAngle:-end_angle / pi * 180
                                  clockwise:YES];
    NSAffineTransform* transform = [NSAffineTransform transform];
    [transform scaleXBy:1 yBy:radius.height / radius.width];
    return path;
}

xaml_result xaml_drawing_context_impl::draw_arc(xaml_drawing_pen const& pen, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    NSBezierPath* arc = path_arc(m_size, region, start_angle, end_angle);
    set_pen(arc, pen);
    [arc stroke];
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_pie(xaml_drawing_brush const& brush, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    NSBezierPath* arc = path_arc(m_size, region, start_angle, end_angle);
    [arc closePath];
    set_brush(arc, brush);
    [arc fill];
    return XAML_S_OK;
}

static NSBezierPath* path_ellipse(xaml_size const& base_size, xaml_rectangle const& region) noexcept
{
    return [NSBezierPath bezierPathWithOvalInRect:NSMakeRect(region.x, base_size.height - region.height - region.y, region.width, region.height)];
}

xaml_result xaml_drawing_context_impl::draw_ellipse(xaml_drawing_pen const& pen, xaml_rectangle const& region) noexcept
{
    NSBezierPath* ellipse = path_ellipse(m_size, region);
    set_pen(ellipse, pen);
    [ellipse stroke];
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_ellipse(xaml_drawing_brush const& brush, xaml_rectangle const& region) noexcept
{
    NSBezierPath* ellipse = path_ellipse(m_size, region);
    set_brush(ellipse, brush);
    [ellipse fill];
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_line(xaml_drawing_pen const& pen, xaml_point const& startp, xaml_point const& endp) noexcept
{
    NSBezierPath* line = [NSBezierPath bezierPath];
    [line moveToPoint:NSMakePoint(startp.x, m_size.height - startp.y)];
    [line lineToPoint:NSMakePoint(endp.x, m_size.height - endp.y)];
    set_pen(line, pen);
    [line stroke];
    return XAML_S_OK;
}

static NSBezierPath* path_rect(xaml_size const& base_size, xaml_rectangle const& rect) noexcept
{
    return [NSBezierPath bezierPathWithRect:NSMakeRect(rect.x, base_size.height - rect.height - rect.y, rect.width, rect.height)];
}

xaml_result xaml_drawing_context_impl::draw_rect(xaml_drawing_pen const& pen, xaml_rectangle const& rect) noexcept
{
    NSBezierPath* path = path_rect(m_size, rect);
    set_pen(path, pen);
    [path stroke];
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_rect(xaml_drawing_brush const& brush, xaml_rectangle const& rect) noexcept
{
    NSBezierPath* path = path_rect(m_size, rect);
    set_brush(path, brush);
    [path fill];
    return XAML_S_OK;
}

static NSBezierPath* path_round_rect(xaml_size const& base_size, xaml_rectangle const& rect, xaml_size const& round) noexcept
{
    return [NSBezierPath bezierPathWithRoundedRect:NSMakeRect(rect.x, base_size.height - rect.height - rect.y, rect.width, rect.height)
                                           xRadius:round.width / 2
                                           yRadius:round.height / 2];
}

xaml_result xaml_drawing_context_impl::draw_round_rect(xaml_drawing_pen const& pen, xaml_rectangle const& rect, xaml_size const& round) noexcept
{
    NSBezierPath* path = path_round_rect(m_size, rect, round);
    set_pen(path, pen);
    [path stroke];
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_round_rect(xaml_drawing_brush const& brush, xaml_rectangle const& rect, xaml_size const& round) noexcept
{
    NSBezierPath* path = path_round_rect(m_size, rect, round);
    set_brush(path, brush);
    [path fill];
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_string(xaml_drawing_brush const& brush, xaml_drawing_font const& font, xaml_point const& p, xaml_string* str) noexcept
{
    NSString* font_name;
    XAML_RETURN_IF_FAILED(get_NSString(font.font_family, &font_name));
    NSFontDescriptor* fontdes = [NSFontDescriptor fontDescriptorWithName:font_name
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
    NSString* data;
    XAML_RETURN_IF_FAILED(get_NSString(str, &data));
    NSAttributedString* astr = [[NSAttributedString alloc] initWithString:data
                                                               attributes:attrs];
    NSSize str_size = astr.size;
    NSPoint location = NSMakePoint(p.x, m_size.height - p.y - str_size.height);
    switch (font.halign)
    {
    case xaml_halignment_center:
        location.x -= str_size.width / 2;
        break;
    case xaml_halignment_right:
        location.x -= str_size.width;
        break;
    default:
        break;
    }
    switch (font.valign)
    {
    case xaml_valignment_center:
        location.y += str_size.height / 2;
        break;
    case xaml_valignment_bottom:
        location.y += str_size.height;
        break;
    default:
        break;
    }
    [astr drawAtPoint:location];
    return XAML_S_OK;
}

xaml_result xaml_canvas_internal::draw(const xaml_rectangle& region) noexcept
{
    if (!m_handle)
    {
        XamlCanvasView* view = [[XamlCanvasView alloc] initWithClassPointer:this];
        m_handle = view;
        XAML_RETURN_IF_FAILED(draw_visible());
    }
    XAML_RETURN_IF_FAILED(set_rect(region));
    XamlCanvasView* view = (XamlCanvasView*)m_handle;
    [view setNeedsDisplay:YES];
    return XAML_S_OK;
}

void xaml_canvas_internal::on_draw_rect() noexcept
{
    xaml_ptr<xaml_drawing_context> dc;
    XAML_ASSERT_SUCCEEDED(xaml_object_new<xaml_drawing_context_impl>(&dc, m_size));
    XAML_ASSERT_SUCCEEDED(on_redraw(m_outer_this, dc));
}
