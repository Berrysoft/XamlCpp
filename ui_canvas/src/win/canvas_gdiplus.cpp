#include <win/canvas_gdiplus.hpp>
#include <windowsx.h>
#include <xaml/ui/win/dark_mode.h>
#include <xaml/ui/win/dpi.h>

#if __has_include(<numbers>)
#include <numbers>
using std::numbers::pi;
#else
inline constexpr double pi = M_PI;
#endif // __has_include(<numbers>)

using namespace std;
using namespace Gdiplus;

constexpr xaml_result xaml_result_from_gdiplus(Gdiplus::Status err) noexcept { return (xaml_result)(err) <= 0 ? (xaml_result)(err) : (xaml_result)((err & 0x0000FFFF) | (FACILITY_UI << 16) | 0x80000000 | 0x20000000); }

#define check_status(expr)                                       \
    do                                                           \
    {                                                            \
        Gdiplus::Status __status = (expr);                       \
        if (__status) return xaml_result_from_gdiplus(__status); \
    } while (0)

static constexpr REAL get_WIDTH(double width, double dpi) noexcept
{
    return (REAL)(width * dpi / USER_DEFAULT_SCREEN_DPI);
}

static inline Pen get_Pen(xaml_drawing_pen const& pen, double dpi) noexcept
{
    return Pen(Color((int)pen.stroke), get_WIDTH(pen.width, dpi));
}

static inline SolidBrush get_Brush(xaml_drawing_brush const& brush) noexcept
{
    return SolidBrush(Color((int)brush.fill));
}

static constexpr INT get_font_style(bool italic, bool bold) noexcept
{
    return (italic ? Gdiplus::FontStyleItalic : Gdiplus::FontStyleRegular) | (bold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular);
}

static inline Font get_Font(xaml_drawing_font const& font, double dpi) noexcept
{
    return Font(font.font_family, get_WIDTH(font.size, dpi), get_font_style(font.italic, font.bold), UnitPixel);
}

static inline RectF get_RectF(xaml_rectangle const& rect, double dpi) noexcept
{
    return xaml_to_native<RectF>(rect * dpi / USER_DEFAULT_SCREEN_DPI);
}

static inline PointF get_PointF(xaml_point p, double dpi) noexcept
{
    return xaml_to_native<PointF>(p * dpi / USER_DEFAULT_SCREEN_DPI);
}

static inline SizeF get_SizeF(xaml_size s, double dpi) noexcept
{
    return xaml_to_native<SizeF>(s * dpi / USER_DEFAULT_SCREEN_DPI);
}

#define CHECK_SIZE(r) \
    if ((r).width < 1 || (r).height < 1) return XAML_S_OK

xaml_result xaml_drawing_context_gdiplus_impl::draw_arc(xaml_drawing_pen const& pen, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    CHECK_SIZE(region);
    auto p = get_Pen(pen, dpi);
    check_status(handle->DrawArc(&p, get_RectF(region, dpi), (REAL)(start_angle / pi * 180), (REAL)((end_angle - start_angle) / pi * 180)));
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_gdiplus_impl::fill_pie(xaml_drawing_brush const& brush, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    CHECK_SIZE(region);
    auto b = get_Brush(brush);
    check_status(handle->FillPie(&b, get_RectF(region, dpi), (REAL)(start_angle / pi * 180), (REAL)((end_angle - start_angle) / pi * 180)));
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_gdiplus_impl::draw_ellipse(drawing_pen const& pen, rectangle const& region)
{
    CHECK_SIZE(region);
    auto p = get_Pen(pen, dpi);
    check_status(handle->DrawEllipse(&p, get_RectF(region, dpi)));
}

xaml_result xaml_drawing_context_gdiplus_impl::fill_ellipse(drawing_brush const& brush, rectangle const& region)
{
    CHECK_SIZE(region);
    auto b = get_Brush(brush);
    check_status(handle->FillEllipse(&b, get_RectF(region, dpi)));
}

xaml_result xaml_drawing_context_gdiplus_impl::draw_line(drawing_pen const& pen, point startp, point endp)
{
    auto p = get_Pen(pen, dpi);
    check_status(handle->DrawLine(&p, get_PointF(startp, dpi), get_PointF(endp, dpi)));
}

xaml_result xaml_drawing_context_gdiplus_impl::draw_rect(drawing_pen const& pen, rectangle const& rect)
{
    CHECK_SIZE(rect);
    auto p = get_Pen(pen, dpi);
    check_status(handle->DrawRectangle(&p, get_RectF(rect, dpi)));
}

xaml_result xaml_drawing_context_gdiplus_impl::fill_rect(drawing_brush const& brush, rectangle const& rect)
{
    CHECK_SIZE(rect);
    auto b = get_Brush(brush);
    check_status(handle->FillRectangle(&b, get_RectF(rect, dpi)));
}

static void rounded_rect(GraphicsPath& path, RectF bounds, SizeF size)
{
    PointF ori_loc;
    bounds.GetLocation(&ori_loc);
    RectF arc{ ori_loc, { size.Width * 2, size.Height * 2 } };
    if (size.Width == 0 && size.Height == 0)
    {
        check_status(path.AddRectangle(bounds));
        return;
    }
    // top left arc
    check_status(path.AddArc(arc, 180, 90));
    // top right arc
    arc.X = bounds.Width + bounds.X - size.Width * 2;
    check_status(path.AddArc(arc, 270, 90));
    // bottom right arc
    arc.Y = bounds.Height + bounds.Y - size.Height * 2;
    check_status(path.AddArc(arc, 0, 90));
    // bottom left arc
    arc.X = bounds.X;
    check_status(path.AddArc(arc, 90, 90));
    check_status(path.CloseFigure());
}

xaml_result xaml_drawing_context_gdiplus_impl::draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round)
{
    CHECK_SIZE(rect);
    CHECK_SIZE(round);
    auto p = get_Pen(pen, dpi);
    GraphicsPath path{};
    rounded_rect(path, get_RectF(rect, dpi), get_SizeF(round, dpi));
    check_status(handle->DrawPath(&p, &path));
}

xaml_result xaml_drawing_context_gdiplus_impl::fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round)
{
    CHECK_SIZE(rect);
    CHECK_SIZE(round);
    auto b = get_Brush(brush);
    GraphicsPath path{};
    rounded_rect(path, get_RectF(rect, dpi), get_SizeF(round, dpi));
    check_status(handle->FillPath(&b, &path));
}

static constexpr StringAlignment get_Align(halignment_t align)
{
    switch (align)
    {
    case halignment_t::center:
        return StringAlignmentCenter;
    case halignment_t::right:
        return StringAlignmentFar;
    default:
        return StringAlignmentNear;
    }
}

xaml_result xaml_drawing_context_gdiplus_impl::draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str)
{
    auto b = get_Brush(brush);
    auto f = get_Font(font, dpi);
    if (f.GetSize() <= 0) return;
    StringFormat fmt{};
    auto a = get_Align(font.halign);
    check_status(fmt.SetAlignment(a));
    check_status(fmt.SetLineAlignment(a));
    auto pf = get_PointF(p, dpi);
    RectF r;
    check_status(handle->MeasureString(str.data(), (INT)str.length(), &f, pf, &fmt, &r));
    switch (font.valign)
    {
    case valignment_t::center:
        pf.Y -= r.Height / 2;
        break;
    case valignment_t::bottom:
        pf.Y -= r.Height;
        break;
    default:
        break;
    }
    check_status(handle->DrawString(str.data(), (INT)str.length(), &f, pf, &fmt, &b));
}

xaml_canvas_gdiplus_impl::~xaml_canvas_gdiplus_impl()
{
    if (m_token) GdiplusShutdown(*m_token);
}

bool canvas_gdiplus::create(HWND wnd) noexcept
{
    GdiplusStartupInput gdiplusStartupInput{};
    ULONG_PTR token;
    if (GdiplusStartup(&token, &gdiplusStartupInput, NULL) == Status::Ok)
    {
        m_token = token;
        return true;
    }
    return false;
}

void canvas_gdiplus::begin_paint(HWND wnd, size real, function<void(shared_ptr<drawing_context>)> paint_func)
{
    UINT dpi = XamlGetDpiForWindow(wnd);
    Graphics g{ wnd };
    Color background_color = XamlIsDarkModeAllowedForApp() ? Color::Black : Color::White;
    check_status(g.Clear(background_color));
    drawing_context_gdiplus ctx{};
    ctx.handle = &g;
    ctx.dpi = (double)dpi;
    auto dc = make_shared<drawing_context>(&ctx);
    paint_func(dc);
}
