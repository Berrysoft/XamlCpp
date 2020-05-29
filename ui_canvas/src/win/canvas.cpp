#include <numbers>
#include <shared/canvas.hpp>
#include <xaml/ui/controls/canvas.h>
#include <xaml/ui/win/controls/brush.h>
#include <xaml/ui/win/controls/pen.h>
#include <xaml/ui/win/dark_mode.h>
#include <xaml/ui/win/dpi.h>

#include <CommCtrl.h>

using namespace std;
using std::numbers::pi;

static constexpr D2D1_COLOR_F get_COLOR(xaml_color c) noexcept
{
    return { c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f };
}

static xaml_result get_Brush(ID2D1RenderTarget* target, xaml_brush* brush, xaml_rectangle const& region, ID2D1Brush** ptr) noexcept
{
    xaml_ptr<xaml_win32_brush> native_brush;
    XAML_RETURN_IF_FAILED(brush->query(&native_brush));
    return native_brush->create(target, region, ptr);
}

static xaml_result get_Brush(ID2D1RenderTarget* target, xaml_pen* pen, xaml_rectangle const& region, ID2D1Brush** ptr, FLOAT* pwidth) noexcept
{
    xaml_ptr<xaml_win32_pen> native_pen;
    XAML_RETURN_IF_FAILED(pen->query(&native_pen));
    return native_pen->create(target, region, ptr, pwidth);
}

static D2D1_ELLIPSE get_ELLIPSE(xaml_rectangle const& region) noexcept
{
    return D2D1::Ellipse(D2D1::Point2F((FLOAT)(region.x + region.width / 2), (FLOAT)(region.y + region.height / 2)), (FLOAT)(region.width / 2), (FLOAT)(region.height / 2));
}

#define CHECK_SIZE(r) \
    if ((r).width < 1 || (r).height < 1) return XAML_S_OK

inline xaml_size xaml_from_native(D2D1_SIZE_F const& s) noexcept
{
    return { (double)s.width, (double)s.height };
}
template <>
inline D2D1_SIZE_F xaml_to_native<D2D1_SIZE_F, xaml_size>(xaml_size const& s) noexcept
{
    return { (float)s.width, (float)s.height };
}

inline xaml_point xaml_from_native(D2D1_POINT_2F const& p) noexcept
{
    return { (double)p.x, (double)p.y };
}
template <>
inline D2D1_POINT_2F xaml_to_native<D2D1_POINT_2F, xaml_point>(xaml_point const& p) noexcept
{
    return { (float)p.x, (float)p.y };
}

inline xaml_rectangle xaml_from_native(D2D1_RECT_F const& r) noexcept
{
    return { (double)r.left, (double)r.top, (double)r.right - (double)r.left, (double)r.bottom - (double)r.top };
}
template <>
inline D2D1_RECT_F xaml_to_native<D2D1_RECT_F, xaml_rectangle>(xaml_rectangle const& r) noexcept
{
    return { (float)r.x, (float)r.y, (float)(r.x + r.width), (float)(r.y + r.height) };
}

static constexpr tuple<xaml_size, xaml_point, xaml_point, xaml_point> get_arc(xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    xaml_size radius = { region.width / 2, region.height / 2 };
    xaml_point centerp = { region.x + radius.width, region.y + radius.height };
    xaml_point startp = centerp + xaml_point{ radius.width * cos(start_angle), radius.height * sin(start_angle) };
    xaml_point endp = centerp + xaml_point{ radius.width * cos(end_angle), radius.height * sin(end_angle) };
    return make_tuple(radius, centerp, startp, endp);
}

static HRESULT get_arc_geo(wil::com_ptr_t<ID2D1Factory, wil::err_returncode_policy> const& d2d, xaml_rectangle const& region, double start_angle, double end_angle, bool close, ID2D1PathGeometry** ptr) noexcept
{
    wil::com_ptr_t<ID2D1PathGeometry, wil::err_returncode_policy> geo;
    RETURN_IF_FAILED(d2d->CreatePathGeometry(&geo));
    wil::com_ptr_t<ID2D1GeometrySink, wil::err_returncode_policy> sink;
    RETURN_IF_FAILED(geo->Open(&sink));
    auto [radius, centerp, startp, endp] = get_arc(region, start_angle, end_angle);
    sink->BeginFigure(xaml_to_native<D2D1_POINT_2F>(startp), D2D1_FIGURE_BEGIN_HOLLOW);
    if (close) sink->AddLine(xaml_to_native<D2D1_POINT_2F>(startp));
    sink->AddArc(D2D1::ArcSegment(xaml_to_native<D2D1_POINT_2F>(endp), xaml_to_native<D2D1_SIZE_F>(radius), 0, D2D1_SWEEP_DIRECTION_CLOCKWISE, ((end_angle - start_angle) > pi) ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL));
    sink->EndFigure(close ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN);
    RETURN_IF_FAILED(sink->Close());
    return geo.copy_to(ptr);
}

xaml_result xaml_drawing_context_impl::draw_arc(xaml_pen* pen, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    CHECK_SIZE(region);
    wil::com_ptr_t<ID2D1PathGeometry, wil::err_returncode_policy> geo;
    XAML_RETURN_IF_FAILED(get_arc_geo(d2d, region, start_angle, end_angle, false, &geo));
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    FLOAT width;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), pen, region, &b, &width));
    target->DrawGeometry(geo.get(), b.get(), width);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_pie(xaml_brush* brush, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    CHECK_SIZE(region);
    wil::com_ptr_t<ID2D1PathGeometry, wil::err_returncode_policy> geo;
    XAML_RETURN_IF_FAILED(get_arc_geo(d2d, region, start_angle, end_angle, false, &geo));
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), brush, region, &b));
    target->FillGeometry(geo.get(), b.get());
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_ellipse(xaml_pen* pen, xaml_rectangle const& region) noexcept
{
    CHECK_SIZE(region);
    auto e = get_ELLIPSE(region);
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    FLOAT width;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), pen, region, &b, &width));
    target->DrawEllipse(e, b.get(), width);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_ellipse(xaml_brush* brush, xaml_rectangle const& region) noexcept
{
    CHECK_SIZE(region);
    auto e = get_ELLIPSE(region);
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), brush, region, &b));
    target->FillEllipse(e, b.get());
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_line(xaml_pen* pen, xaml_point const& startp, xaml_point const& endp) noexcept
{
    xaml_point minp{ (min)(startp.x, endp.x), (min)(startp.y, endp.y) };
    xaml_point maxp{ (max)(startp.x, endp.x), (max)(startp.y, endp.y) };
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    FLOAT width;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), pen, { minp.x, minp.y, maxp.x - minp.x, maxp.y - minp.y }, &b, &width));
    target->DrawLine(xaml_to_native<D2D1_POINT_2F>(startp), xaml_to_native<D2D1_POINT_2F>(endp), b.get(), width);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_rect(xaml_pen* pen, xaml_rectangle const& rect) noexcept
{
    CHECK_SIZE(rect);
    auto r = xaml_to_native<D2D1_RECT_F>(rect);
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    FLOAT width;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), pen, rect, &b, &width));
    target->DrawRectangle(r, b.get(), width);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_rect(xaml_brush* brush, xaml_rectangle const& rect) noexcept
{
    CHECK_SIZE(rect);
    auto r = xaml_to_native<D2D1_RECT_F>(rect);
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), brush, rect, &b));
    target->FillRectangle(r, b.get());
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_round_rect(xaml_pen* pen, xaml_rectangle const& rect, xaml_size const& round) noexcept
{
    CHECK_SIZE(rect);
    CHECK_SIZE(round);
    auto r = xaml_to_native<D2D1_RECT_F>(rect);
    auto s = xaml_to_native<D2D1_SIZE_F>(round);
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    FLOAT width;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), pen, rect, &b, &width));
    target->DrawRoundedRectangle(D2D1::RoundedRect(r, s.width, s.height), b.get(), width);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_round_rect(xaml_brush* brush, xaml_rectangle const& rect, xaml_size const& round) noexcept
{
    CHECK_SIZE(rect);
    CHECK_SIZE(round);
    auto r = xaml_to_native<D2D1_RECT_F>(rect);
    auto s = xaml_to_native<D2D1_SIZE_F>(round);
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), brush, rect, &b));
    target->FillRoundedRectangle(D2D1::RoundedRect(r, s.width, s.height), b.get());
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_string(xaml_brush* brush, xaml_drawing_font const& font, xaml_point const& p, xaml_string* str) noexcept
{
    auto fsize = (FLOAT)font.size;
    if (fsize <= 0) return XAML_S_OK;
    wstring ff;
    XAML_RETURN_IF_FAILED(to_wstring(font.font_family, &ff));
    wil::com_ptr_t<IDWriteTextFormat, wil::err_returncode_policy> format;
    XAML_RETURN_IF_FAILED(dwrite->CreateTextFormat(
        ff.c_str(), nullptr,
        font.bold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
        font.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL, fsize, L"", &format));
    auto size = target->GetSize();
    auto region = xaml_to_native<D2D1_RECT_F, xaml_rectangle>({ p.x, p.y, p.x, p.y });
    wstring data;
    XAML_RETURN_IF_FAILED(to_wstring(str, &data));
    wil::com_ptr_t<IDWriteTextLayout, wil::err_returncode_policy> layout;
    XAML_RETURN_IF_FAILED(dwrite->CreateTextLayout(data.c_str(), (UINT32)data.length(), format.get(), size.width, size.height, &layout));
    DWRITE_TEXT_METRICS metrics;
    XAML_RETURN_IF_FAILED(layout->GetMetrics(&metrics));
    switch (font.halign)
    {
    case xaml_halignment_center:
        region.left -= metrics.width / 2;
        break;
    case xaml_halignment_right:
        region.left -= metrics.width;
        break;
    default:
        break;
    }
    switch (font.valign)
    {
    case xaml_valignment_center:
        region.top -= metrics.height / 2;
        break;
    case xaml_valignment_bottom:
        region.top -= metrics.height;
        break;
    default:
        break;
    }
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), brush, { region.left, region.top, metrics.width, metrics.height }, &b));
    target->DrawTextLayout(D2D1::Point2F(region.left, region.top), layout.get(), b.get());
    return XAML_S_OK;
}

xaml_result xaml_canvas_internal::wnd_proc(xaml_win32_window_message const& msg, LRESULT* presult) noexcept
{
    switch (msg.Msg)
    {
    case WM_DRAWITEM:
    {
        DRAWITEMSTRUCT* ds = (DRAWITEMSTRUCT*)msg.lParam;
        if (ds->hwndItem == m_handle)
        {
            UINT dpi = XamlGetDpiForWindow(m_handle);
            xaml_size region = m_size * dpi / USER_DEFAULT_SCREEN_DPI;
            CHECK_SIZE(region);
            target->Resize(D2D1::SizeU((UINT32)region.width, (UINT32)region.height));
            target->BeginDraw();
            target->SetDpi((FLOAT)dpi, (FLOAT)dpi);
            D2D1_COLOR_F background_color = XamlIsDarkModeAllowedForApp() ? D2D1::ColorF(D2D1::ColorF::Black) : D2D1::ColorF(D2D1::ColorF::White);
            target->Clear(background_color);
            wil::com_ptr_t<ID2D1RenderTarget, wil::err_returncode_policy> ctx_target;
            XAML_RETURN_IF_FAILED(target.query_to(&ctx_target));
            xaml_ptr<xaml_drawing_context> dc;
            XAML_RETURN_IF_FAILED(xaml_object_new<xaml_drawing_context_impl>(&dc, ctx_target, d2d, dwrite));
            XAML_RETURN_IF_FAILED(on_redraw(m_outer_this, dc));
            XAML_RETURN_IF_FAILED(target->EndDraw());
        }
        *presult = TRUE;
        return XAML_S_OK;
    }
    }
    return XAML_E_NOTIMPL;
}

xaml_result XAML_CALL xaml_canvas_internal::draw(xaml_rectangle const& region) noexcept
{
    if (m_parent)
    {
        if (!m_handle)
        {
            xaml_win32_window_create_params params = {};
            params.class_name = WC_STATIC;
            params.style = WS_CHILD | WS_VISIBLE | SS_OWNERDRAW;
            params.x = 0;
            params.y = 0;
            params.width = 100;
            params.height = 50;
            params.parent = m_parent;
            XAML_RETURN_IF_FAILED(create(params));
            XAML_RETURN_IF_FAILED(draw_visible());
        }
        XAML_RETURN_IF_FAILED(set_rect(region));
        if (!target)
        {
            XAML_RETURN_IF_FAILED(d2d->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(m_handle), &target));
        }
    }
    return XAML_S_OK;
}
