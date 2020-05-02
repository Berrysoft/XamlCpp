#ifdef XAML_UI_CANVAS_DIRECT2D

#include <shared/canvas.hpp>
#include <tuple>
#include <wil/result_macros.h>
#include <win/canvas_d2d.hpp>
#include <xaml/ui/win/dark_mode.h>
#include <xaml/ui/win/dpi.h>

#if __has_include(<numbers>)
#include <numbers>
using std::numbers::pi;
#else
inline constexpr double pi = M_PI;
#endif // __has_include(<numbers>)

using namespace std;

static inline D2D1::ColorF get_COLOR(xaml_color c) noexcept
{
    return (uint32_t)c;
}

static HRESULT get_Brush(ID2D1RenderTarget* target, xaml_color c, ID2D1Brush** ptr) noexcept
{
    wil::com_ptr_t<ID2D1SolidColorBrush, wil::err_returncode_policy> brush;
    RETURN_IF_FAILED(target->CreateSolidColorBrush(get_COLOR(c), &brush));
    return brush.copy_to(ptr);
}

static D2D1_ELLIPSE get_ELLIPSE(xaml_rectangle const& region) noexcept
{
    return D2D1::Ellipse(D2D1::Point2F((FLOAT)(region.x + region.width / 2), (FLOAT)(region.y + region.height / 2)), (FLOAT)(region.width / 2), (FLOAT)(region.height / 2));
}

#define CHECK_SIZE(r) \
    if ((r).width < 1 || (r).height < 1) return XAML_S_OK

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

xaml_result xaml_drawing_context_d2d_impl::draw_arc(xaml_drawing_pen const& pen, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    CHECK_SIZE(region);
    wil::com_ptr_t<ID2D1PathGeometry, wil::err_returncode_policy> geo;
    XAML_RETURN_IF_FAILED(get_arc_geo(d2d, region, start_angle, end_angle, false, &geo));
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), pen.stroke, &b));
    target->DrawGeometry(geo.get(), b.get(), (FLOAT)pen.width);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_d2d_impl::fill_pie(xaml_drawing_brush const& brush, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    CHECK_SIZE(region);
    wil::com_ptr_t<ID2D1PathGeometry, wil::err_returncode_policy> geo;
    XAML_RETURN_IF_FAILED(get_arc_geo(d2d, region, start_angle, end_angle, false, &geo));
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), brush.fill, &b));
    target->FillGeometry(geo.get(), b.get());
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_d2d_impl::draw_ellipse(xaml_drawing_pen const& pen, xaml_rectangle const& region) noexcept
{
    CHECK_SIZE(region);
    auto e = get_ELLIPSE(region);
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), pen.stroke, &b));
    target->DrawEllipse(e, b.get(), (FLOAT)pen.width);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_d2d_impl::fill_ellipse(xaml_drawing_brush const& brush, xaml_rectangle const& region) noexcept
{
    CHECK_SIZE(region);
    auto e = get_ELLIPSE(region);
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), brush.fill, &b));
    target->FillEllipse(e, b.get());
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_d2d_impl::draw_line(xaml_drawing_pen const& pen, xaml_point const& startp, xaml_point const& endp) noexcept
{
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), pen.stroke, &b));
    target->DrawLine(xaml_to_native<D2D1_POINT_2F>(startp), xaml_to_native<D2D1_POINT_2F>(endp), b.get(), (FLOAT)pen.width);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_d2d_impl::draw_rect(xaml_drawing_pen const& pen, xaml_rectangle const& rect) noexcept
{
    CHECK_SIZE(rect);
    auto r = xaml_to_native<D2D1_RECT_F>(rect);
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), pen.stroke, &b));
    target->DrawRectangle(r, b.get(), (FLOAT)pen.width);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_d2d_impl::fill_rect(xaml_drawing_brush const& brush, xaml_rectangle const& rect) noexcept
{
    CHECK_SIZE(rect);
    auto r = xaml_to_native<D2D1_RECT_F>(rect);
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), brush.fill, &b));
    target->FillRectangle(r, b.get());
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_d2d_impl::draw_round_rect(xaml_drawing_pen const& pen, xaml_rectangle const& rect, xaml_size const& round) noexcept
{
    CHECK_SIZE(rect);
    CHECK_SIZE(round);
    auto r = xaml_to_native<D2D1_RECT_F>(rect);
    auto s = xaml_to_native<D2D1_SIZE_F>(round);
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), pen.stroke, &b));
    target->DrawRoundedRectangle(D2D1::RoundedRect(r, s.width, s.height), b.get(), (FLOAT)pen.width);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_d2d_impl::fill_round_rect(xaml_drawing_brush const& brush, xaml_rectangle const& rect, xaml_size const& round) noexcept
{
    CHECK_SIZE(rect);
    CHECK_SIZE(round);
    auto r = xaml_to_native<D2D1_RECT_F>(rect);
    auto s = xaml_to_native<D2D1_SIZE_F>(round);
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), brush.fill, &b));
    target->FillRoundedRectangle(D2D1::RoundedRect(r, s.width, s.height), b.get());
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_d2d_impl::draw_string(xaml_drawing_brush const& brush, xaml_drawing_font const& font, xaml_point const& p, xaml_string* str) noexcept
{
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), brush.fill, &b));
    auto fsize = (FLOAT)font.size;
    if (fsize <= 0) return XAML_S_OK;
    wil::com_ptr_t<IDWriteTextFormat, wil::err_returncode_policy> format;
    XAML_RETURN_IF_FAILED(dwrite->CreateTextFormat(
        font.font_family, nullptr,
        font.bold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
        font.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL, fsize, L"", &format));
    auto size = target->GetSize();
    auto region = xaml_to_native<D2D1_RECT_F, xaml_rectangle>({ p.x, p.y, p.x, p.y });
    xaml_char_t const* data;
    XAML_RETURN_IF_FAILED(str->get_data(&data));
    int32_t length;
    XAML_RETURN_IF_FAILED(str->get_length(&length));
    wil::com_ptr_t<IDWriteTextLayout, wil::err_returncode_policy> layout;
    XAML_RETURN_IF_FAILED(dwrite->CreateTextLayout(data, (UINT32)length, format.get(), size.width, size.height, &layout));
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
    target->DrawTextLayout(D2D1::Point2F(region.left, region.top), layout.get(), b.get());
    return XAML_S_OK;
}

xaml_result xaml_canvas_d2d_impl::wnd_proc(xaml_win32_window_message const& msg, LRESULT* presult) noexcept
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
            XAML_RETURN_IF_FAILED(xaml_object_new<xaml_drawing_context_d2d_impl>(&dc, ctx_target, d2d, dwrite));
            XAML_RETURN_IF_FAILED(on_redraw(this, dc));
            XAML_RETURN_IF_FAILED(target->EndDraw());
        }
        *presult = TRUE;
        return XAML_S_OK;
    }
    }
    return XAML_E_NOTIMPL;
}

xaml_result xaml_canvas_d2d_impl::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_canvas_implement::init());
    XAML_RETURN_IF_FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d));
    XAML_RETURN_IF_FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, &dwrite));
    return XAML_S_OK;
}

xaml_result xaml_canvas_d2d_impl::draw_impl() noexcept
{
    if (!target)
    {
        XAML_RETURN_IF_FAILED(d2d->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(m_handle), &target));
    }
    return XAML_S_OK;
}

#endif // XAML_UI_CANVAS_DIRECT2D
