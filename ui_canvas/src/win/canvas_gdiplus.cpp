#include <numbers>
#include <win/canvas_gdiplus.hpp>
#include <windowsx.h>
#include <xaml/ui/win/dark_mode.h>
#include <xaml/ui/win/dpi.h>

using namespace std;
using std::numbers::pi;
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

xaml_result xaml_drawing_context_gdiplus_impl::draw_ellipse(xaml_drawing_pen const& pen, xaml_rectangle const& region) noexcept
{
    CHECK_SIZE(region);
    auto p = get_Pen(pen, dpi);
    check_status(handle->DrawEllipse(&p, get_RectF(region, dpi)));
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_gdiplus_impl::fill_ellipse(xaml_drawing_brush const& brush, xaml_rectangle const& region) noexcept
{
    CHECK_SIZE(region);
    auto b = get_Brush(brush);
    check_status(handle->FillEllipse(&b, get_RectF(region, dpi)));
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_gdiplus_impl::draw_line(xaml_drawing_pen const& pen, xaml_point const& startp, xaml_point const& endp) noexcept
{
    auto p = get_Pen(pen, dpi);
    check_status(handle->DrawLine(&p, get_PointF(startp, dpi), get_PointF(endp, dpi)));
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_gdiplus_impl::draw_rect(xaml_drawing_pen const& pen, xaml_rectangle const& rect) noexcept
{
    CHECK_SIZE(rect);
    auto p = get_Pen(pen, dpi);
    check_status(handle->DrawRectangle(&p, get_RectF(rect, dpi)));
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_gdiplus_impl::fill_rect(xaml_drawing_brush const& brush, xaml_rectangle const& rect) noexcept
{
    CHECK_SIZE(rect);
    auto b = get_Brush(brush);
    check_status(handle->FillRectangle(&b, get_RectF(rect, dpi)));
    return XAML_S_OK;
}

static xaml_result rounded_rect(GraphicsPath& path, RectF bounds, SizeF size) noexcept
{
    PointF ori_loc;
    bounds.GetLocation(&ori_loc);
    RectF arc{ ori_loc, { size.Width * 2, size.Height * 2 } };
    if (size.Width == 0 && size.Height == 0)
    {
        check_status(path.AddRectangle(bounds));
        return XAML_S_OK;
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
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_gdiplus_impl::draw_round_rect(xaml_drawing_pen const& pen, xaml_rectangle const& rect, xaml_size const& round) noexcept
{
    CHECK_SIZE(rect);
    CHECK_SIZE(round);
    auto p = get_Pen(pen, dpi);
    GraphicsPath path{};
    XAML_RETURN_IF_FAILED(rounded_rect(path, get_RectF(rect, dpi), get_SizeF(round, dpi)));
    check_status(handle->DrawPath(&p, &path));
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_gdiplus_impl::fill_round_rect(xaml_drawing_brush const& brush, xaml_rectangle const& rect, xaml_size const& round) noexcept
{
    CHECK_SIZE(rect);
    CHECK_SIZE(round);
    auto b = get_Brush(brush);
    GraphicsPath path{};
    XAML_RETURN_IF_FAILED(rounded_rect(path, get_RectF(rect, dpi), get_SizeF(round, dpi)));
    check_status(handle->FillPath(&b, &path));
    return XAML_S_OK;
}

static constexpr StringAlignment get_Align(xaml_halignment align) noexcept
{
    switch (align)
    {
    case xaml_halignment_center:
        return StringAlignmentCenter;
    case xaml_halignment_right:
        return StringAlignmentFar;
    default:
        return StringAlignmentNear;
    }
}

xaml_result xaml_drawing_context_gdiplus_impl::draw_string(xaml_drawing_brush const& brush, xaml_drawing_font const& font, xaml_point const& p, xaml_string* str) noexcept
{
    auto b = get_Brush(brush);
    auto f = get_Font(font, dpi);
    if (f.GetSize() <= 0) return XAML_S_OK;
    StringFormat fmt{};
    auto a = get_Align(font.halign);
    check_status(fmt.SetAlignment(a));
    check_status(fmt.SetLineAlignment(a));
    auto pf = get_PointF(p, dpi);
    xaml_char_t const* data;
    XAML_RETURN_IF_FAILED(str->get_data(&data));
    int32_t length;
    XAML_RETURN_IF_FAILED(str->get_length(&length));
    RectF r;
    check_status(handle->MeasureString(data, length, &f, pf, &fmt, &r));
    switch (font.valign)
    {
    case xaml_valignment_center:
        pf.Y -= r.Height / 2;
        break;
    case xaml_valignment_bottom:
        pf.Y -= r.Height;
        break;
    default:
        break;
    }
    check_status(handle->DrawString(data, length, &f, pf, &fmt, &b));
    return XAML_S_OK;
}

xaml_canvas_gdiplus_internal::~xaml_canvas_gdiplus_internal()
{
    if (m_token) GdiplusShutdown(*m_token);
}

xaml_result xaml_canvas_gdiplus_internal::wnd_proc(xaml_win32_window_message const& msg, LRESULT* presult) noexcept
{
    switch (msg.Msg)
    {
    case WM_DRAWITEM:
    {
        DRAWITEMSTRUCT* ds = (DRAWITEMSTRUCT*)msg.lParam;
        if (ds->hwndItem == m_handle)
        {
            UINT dpi = XamlGetDpiForWindow(m_handle);
            Graphics g{ m_handle };
            Color background_color = XamlIsDarkModeAllowedForApp() ? Color::Black : Color::White;
            check_status(g.Clear(background_color));
            xaml_ptr<xaml_drawing_context> dc;
            XAML_RETURN_IF_FAILED(xaml_object_new<xaml_drawing_context_gdiplus_impl>(&dc, &g, (double)dpi));
            XAML_RETURN_IF_FAILED(on_redraw(m_outer_this, dc));
        }
        *presult = TRUE;
        return XAML_S_OK;
    }
    }
    return XAML_E_NOTIMPL;
}

xaml_result xaml_canvas_gdiplus_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_canvas_internal::init());
    GdiplusStartupInput input{};
    ULONG_PTR token;
    check_status(GdiplusStartup(&token, &input, nullptr));
    m_token = token;
    return XAML_S_OK;
}

xaml_result xaml_canvas_gdiplus_internal::draw_impl() noexcept
{
    return XAML_S_OK;
}
