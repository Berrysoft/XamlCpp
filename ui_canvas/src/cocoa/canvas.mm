#include <cmath>
#import <cocoa/XamlCanvasView.h>
#include <cocoa/drawing_mask.hpp>
#include <cocoa/nsstring.hpp>
#include <functional>
#include <shared/canvas.hpp>
#include <tuple>
#include <xaml/ui/cocoa/controls/brush.h>
#include <xaml/ui/cocoa/controls/pen.h>
#include <xaml/ui/controls/canvas.h>
#include <xaml/ui/drawing_conv.hpp>

static xaml_mouse_button get_mouse_button(NSEvent* event) noexcept
{
    switch (event.type)
    {
    case NSEventTypeLeftMouseDown:
    case NSEventTypeLeftMouseUp:
        return xaml_mouse_button_left;
    case NSEventTypeRightMouseDown:
    case NSEventTypeRightMouseUp:
        return xaml_mouse_button_right;
    default:
        return xaml_mouse_button_other;
    }
}

static xaml_point get_mouse_location(NSView* view) noexcept
{
    return xaml_from_native([view.window mouseLocationOutsideOfEventStream]);
}

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

- (void)updateTrackingAreas
{
    if (self->area)
    {
        [self removeTrackingArea:self->area];
    }
    self->area = [[NSTrackingArea alloc] initWithRect:[self bounds]
                                              options:NSTrackingMouseMoved | NSTrackingActiveAlways
                                                owner:self
                                             userInfo:nil];
    [self addTrackingArea:self->area];
}

- (void)drawRect:(NSRect)dirtyRect
{
    xaml_canvas_internal* cv = (xaml_canvas_internal*)self.classPointer;
    cv->on_draw_rect();
}

- (void)mouseDown:(NSEvent*)event
{
    xaml_canvas_internal* cv = (xaml_canvas_internal*)self.classPointer;
    cv->on_mouse_down_event(get_mouse_button(event));
}

- (void)mouseUp:(NSEvent*)event
{
    xaml_canvas_internal* cv = (xaml_canvas_internal*)self.classPointer;
    cv->on_mouse_up_event(get_mouse_button(event));
}

- (void)mouseDragged:(NSEvent*)event
{
    xaml_canvas_internal* cv = (xaml_canvas_internal*)self.classPointer;
    cv->on_mouse_moved_event(get_mouse_location(self));
}

- (void)mouseMoved:(NSEvent*)event
{
    xaml_canvas_internal* cv = (xaml_canvas_internal*)self.classPointer;
    cv->on_mouse_moved_event(get_mouse_location(self));
}
@end

#if __has_include(<numbers>)
    #include <numbers>
using std::numbers::pi;
#else
    #define pi M_PI
#endif // __has_include(<numbers>)

using namespace std;

static xaml_result set_pen(NSBezierPath* path, xaml_pen* pen, xaml_size const& size, xaml_rectangle const& region) noexcept
{
    xaml_ptr<xaml_cocoa_pen> native_pen;
    XAML_RETURN_IF_FAILED(pen->query(&native_pen));
    return native_pen->draw(path, size, region);
}

static xaml_result set_brush(NSBezierPath* path, xaml_brush* brush, xaml_size const& size, xaml_rectangle const& region) noexcept
{
    xaml_ptr<xaml_cocoa_brush> native_brush;
    XAML_RETURN_IF_FAILED(brush->query(&native_brush));
    return native_brush->draw(path, size, region);
}

static NSBezierPath* path_arc(xaml_size const& base_size, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    xaml_size radius = { region.width / 2, region.height / 2 };
    xaml_point centerp = { region.x + radius.width, region.y + radius.height };
    xaml_point startp = centerp + xaml_point{ radius.width * cos(start_angle), radius.height * sin(start_angle) };
    double rate = radius.height / radius.width;
    NSAffineTransform* transform = [NSAffineTransform transform];
    [transform scaleXBy:1 yBy:rate];
    [transform concat];
    NSBezierPath* path = [NSBezierPath bezierPath];
    [path moveToPoint:NSMakePoint(startp.x, (base_size.height - startp.y) / rate)];
    [path appendBezierPathWithArcWithCenter:NSMakePoint(centerp.x, (base_size.height - centerp.y) / rate)
                                     radius:radius.width
                                 startAngle:-start_angle / pi * 180
                                   endAngle:-end_angle / pi * 180
                                  clockwise:YES];
    return path;
}

static constexpr tuple<xaml_size, xaml_rectangle> get_scaled_rect(xaml_size const& size, xaml_rectangle const& region) noexcept
{
    xaml_rectangle scaled_region = region;
    double rate = scaled_region.height / scaled_region.width;
    scaled_region.y /= rate;
    scaled_region.height /= rate;
    xaml_size scaled_size = { size.width, size.height / rate };
    return make_tuple(scaled_size, scaled_region);
}

xaml_result xaml_drawing_context_impl::draw_arc(xaml_pen* pen, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    [[NSGraphicsContext currentContext] saveGraphicsState];
    NSBezierPath* arc = path_arc(m_size, region, start_angle, end_angle);
    auto [scaled_size, scaled_region] = get_scaled_rect(m_size, region);
    xaml_result hr = set_pen(arc, pen, scaled_size, scaled_region);
    [[NSGraphicsContext currentContext] restoreGraphicsState];
    return hr;
}

xaml_result xaml_drawing_context_impl::fill_pie(xaml_brush* brush, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    [[NSGraphicsContext currentContext] saveGraphicsState];
    NSBezierPath* arc = path_arc(m_size, region, start_angle, end_angle);
    [arc closePath];
    auto [scaled_size, scaled_region] = get_scaled_rect(m_size, region);
    xaml_result hr = set_brush(arc, brush, scaled_size, scaled_region);
    [[NSGraphicsContext currentContext] restoreGraphicsState];
    return hr;
}

static NSBezierPath* path_ellipse(xaml_size const& base_size, xaml_rectangle const& region) noexcept
{
    return [NSBezierPath bezierPathWithOvalInRect:NSMakeRect(region.x, base_size.height - region.height - region.y, region.width, region.height)];
}

xaml_result xaml_drawing_context_impl::draw_ellipse(xaml_pen* pen, xaml_rectangle const& region) noexcept
{
    NSBezierPath* ellipse = path_ellipse(m_size, region);
    return set_pen(ellipse, pen, m_size, region);
}

xaml_result xaml_drawing_context_impl::fill_ellipse(xaml_brush* brush, xaml_rectangle const& region) noexcept
{
    NSBezierPath* ellipse = path_ellipse(m_size, region);
    return set_brush(ellipse, brush, m_size, region);
}

xaml_result xaml_drawing_context_impl::draw_line(xaml_pen* pen, xaml_point const& startp, xaml_point const& endp) noexcept
{
    xaml_point minp{ (min)(startp.x, endp.x), (min)(startp.y, endp.y) };
    xaml_point maxp{ (max)(startp.x, endp.x), (max)(startp.y, endp.y) };
    NSBezierPath* line = [NSBezierPath bezierPath];
    [line moveToPoint:NSMakePoint(startp.x, m_size.height - startp.y)];
    [line lineToPoint:NSMakePoint(endp.x, m_size.height - endp.y)];
    return set_pen(line, pen, m_size, { minp.x, minp.y, maxp.x - minp.x, maxp.y - minp.y });
}

static NSBezierPath* path_rect(xaml_size const& base_size, xaml_rectangle const& rect) noexcept
{
    return [NSBezierPath bezierPathWithRect:NSMakeRect(rect.x, base_size.height - rect.height - rect.y, rect.width, rect.height)];
}

xaml_result xaml_drawing_context_impl::draw_rect(xaml_pen* pen, xaml_rectangle const& rect) noexcept
{
    NSBezierPath* path = path_rect(m_size, rect);
    return set_pen(path, pen, m_size, rect);
}

xaml_result xaml_drawing_context_impl::fill_rect(xaml_brush* brush, xaml_rectangle const& rect) noexcept
{
    NSBezierPath* path = path_rect(m_size, rect);
    return set_brush(path, brush, m_size, rect);
}

static NSBezierPath* path_round_rect(xaml_size const& base_size, xaml_rectangle const& rect, xaml_size const& round) noexcept
{
    return [NSBezierPath bezierPathWithRoundedRect:NSMakeRect(rect.x, base_size.height - rect.height - rect.y, rect.width, rect.height)
                                           xRadius:round.width
                                           yRadius:round.height];
}

xaml_result xaml_drawing_context_impl::draw_round_rect(xaml_pen* pen, xaml_rectangle const& rect, xaml_size const& round) noexcept
{
    NSBezierPath* path = path_round_rect(m_size, rect, round);
    return set_pen(path, pen, m_size, rect);
}

xaml_result xaml_drawing_context_impl::fill_round_rect(xaml_brush* brush, xaml_rectangle const& rect, xaml_size const& round) noexcept
{
    NSBezierPath* path = path_round_rect(m_size, rect, round);
    return set_brush(path, brush, m_size, rect);
}

static xaml_result measure_string_impl(xaml_drawing_font const& font, xaml_point const& p, xaml_string* str, xaml_rectangle* pvalue, NSAttributedString** pastr) noexcept
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
    if (!nfont) return XAML_S_OK;
    NSDictionary* attrs = @{
        NSFontAttributeName : nfont,
        NSForegroundColorAttributeName : [NSColor whiteColor]
    };
    NSString* data;
    XAML_RETURN_IF_FAILED(get_NSString(str, &data));
    NSAttributedString* astr = [[NSAttributedString alloc] initWithString:data
                                                               attributes:attrs];
    NSSize str_size = astr.size;
    auto [x, y] = p;
    switch (font.halign)
    {
    case xaml_halignment_center:
        x -= str_size.width / 2;
        break;
    case xaml_halignment_right:
        x -= str_size.width;
        break;
    default:
        break;
    }
    switch (font.valign)
    {
    case xaml_valignment_center:
        y -= str_size.height / 2;
        break;
    case xaml_valignment_bottom:
        y -= str_size.height;
        break;
    default:
        break;
    }
    *pvalue = { x, y, str_size.width, str_size.height };
    if (pastr) *pastr = astr;
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_string(xaml_brush* brush, xaml_drawing_font const& font, xaml_point const& p, xaml_string* str) noexcept
{
    xaml_rectangle rect;
    NSAttributedString* astr;
    XAML_RETURN_IF_FAILED(measure_string_impl(font, p, str, &rect, &astr));
    auto location = NSMakePoint(rect.x, m_size.height - rect.y - rect.height);
    return draw_mask(
        m_size,
        [astr, &location]() noexcept -> xaml_result {
            [astr drawAtPoint:location];
            return XAML_S_OK;
        },
        [this, brush, &rect]() noexcept -> xaml_result {
            return fill_rect(brush, rect);
        });
}

xaml_result xaml_drawing_context_impl::measure_string(xaml_drawing_font const& font, xaml_point const& p, xaml_string* str, xaml_rectangle* pvalue) noexcept
{
    return measure_string_impl(font, p, str, pvalue, nullptr);
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
    XAML_ASSERT_SUCCEEDED(m_redraw->invoke(m_outer_this, dc));
}

xaml_result xaml_canvas_internal::invalidate(const xaml_rectangle* prect) noexcept
{
    if (prect)
    {
        NSRect r = xaml_to_native<NSRect>(*prect);
        [m_handle setNeedsDisplayInRect:r];
    }
    else
    {
        [m_handle setNeedsDisplay:YES];
    }
    return XAML_S_OK;
}
