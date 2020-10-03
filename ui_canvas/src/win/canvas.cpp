#include <numbers>
#include <shared/canvas.hpp>
#include <win/d2d_conv.hpp>
#include <windowsx.h>
#include <xaml/internal/string.hpp>
#include <xaml/result_win32.h>
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

static constexpr tuple<xaml_size, xaml_point, xaml_point, xaml_point> get_arc(xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    xaml_size radius = { region.width / 2, region.height / 2 };
    xaml_point centerp = { region.x + radius.width, region.y + radius.height };
    xaml_point startp = centerp + xaml_point{ radius.width * cos(start_angle), radius.height * sin(start_angle) };
    xaml_point endp = centerp + xaml_point{ radius.width * cos(end_angle), radius.height * sin(end_angle) };
    return make_tuple(radius, centerp, startp, endp);
}

static HRESULT get_arc_geo(wil::com_ptr_nothrow<ID2D1Factory> const& d2d, xaml_rectangle const& region, double start_angle, double end_angle, bool close, ID2D1PathGeometry** ptr) noexcept
{
    wil::com_ptr_nothrow<ID2D1PathGeometry> geo;
    RETURN_IF_FAILED(d2d->CreatePathGeometry(&geo));
    wil::com_ptr_nothrow<ID2D1GeometrySink> sink;
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
    wil::com_ptr_nothrow<ID2D1PathGeometry> geo;
    XAML_RETURN_IF_FAILED(get_arc_geo(d2d, region, start_angle, end_angle, false, &geo));
    wil::com_ptr_nothrow<ID2D1Brush> b;
    FLOAT width;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), pen, region, &b, &width));
    target->DrawGeometry(geo.get(), b.get(), width);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_pie(xaml_brush* brush, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    CHECK_SIZE(region);
    wil::com_ptr_nothrow<ID2D1PathGeometry> geo;
    XAML_RETURN_IF_FAILED(get_arc_geo(d2d, region, start_angle, end_angle, false, &geo));
    wil::com_ptr_nothrow<ID2D1Brush> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), brush, region, &b));
    target->FillGeometry(geo.get(), b.get());
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_ellipse(xaml_pen* pen, xaml_rectangle const& region) noexcept
{
    CHECK_SIZE(region);
    auto e = get_ELLIPSE(region);
    wil::com_ptr_nothrow<ID2D1Brush> b;
    FLOAT width;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), pen, region, &b, &width));
    target->DrawEllipse(e, b.get(), width);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_ellipse(xaml_brush* brush, xaml_rectangle const& region) noexcept
{
    CHECK_SIZE(region);
    auto e = get_ELLIPSE(region);
    wil::com_ptr_nothrow<ID2D1Brush> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), brush, region, &b));
    target->FillEllipse(e, b.get());
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_line(xaml_pen* pen, xaml_point const& startp, xaml_point const& endp) noexcept
{
    xaml_point minp{ (min)(startp.x, endp.x), (min)(startp.y, endp.y) };
    xaml_point maxp{ (max)(startp.x, endp.x), (max)(startp.y, endp.y) };
    wil::com_ptr_nothrow<ID2D1Brush> b;
    FLOAT width;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), pen, { minp.x, minp.y, maxp.x - minp.x, maxp.y - minp.y }, &b, &width));
    target->DrawLine(xaml_to_native<D2D1_POINT_2F>(startp), xaml_to_native<D2D1_POINT_2F>(endp), b.get(), width);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_rect(xaml_pen* pen, xaml_rectangle const& rect) noexcept
{
    CHECK_SIZE(rect);
    auto r = xaml_to_native<D2D1_RECT_F>(rect);
    wil::com_ptr_nothrow<ID2D1Brush> b;
    FLOAT width;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), pen, rect, &b, &width));
    target->DrawRectangle(r, b.get(), width);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_rect(xaml_brush* brush, xaml_rectangle const& rect) noexcept
{
    CHECK_SIZE(rect);
    auto r = xaml_to_native<D2D1_RECT_F>(rect);
    wil::com_ptr_nothrow<ID2D1Brush> b;
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
    wil::com_ptr_nothrow<ID2D1Brush> b;
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
    wil::com_ptr_nothrow<ID2D1Brush> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), brush, rect, &b));
    target->FillRoundedRectangle(D2D1::RoundedRect(r, s.width, s.height), b.get());
    return XAML_S_OK;
}

static xaml_result measure_string_impl(ID2D1RenderTarget* target, IDWriteFactory* dwrite, xaml_drawing_font const& font, xaml_point const& p, xaml_string* str, xaml_rectangle* pvalue, IDWriteTextLayout** playout) noexcept
{
    auto fsize = (FLOAT)font.size;
    nowide::wshort_stackstring ff;
    XAML_RETURN_IF_FAILED(to_wstring(font.font_family, &ff));
    wil::com_ptr_nothrow<IDWriteTextFormat> format;
    XAML_RETURN_IF_FAILED(dwrite->CreateTextFormat(
        ff.c_str(), nullptr,
        font.bold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
        font.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL, fsize, L"", &format));
    auto size = target->GetSize();
    xaml_rectangle region = { p.x, p.y, p.x, p.y };
    nowide::wstackstring data;
    XAML_RETURN_IF_FAILED(to_wstring(str, &data));
    wil::com_ptr_nothrow<IDWriteTextLayout> layout;
    XAML_RETURN_IF_FAILED(dwrite->CreateTextLayout(data.c_str(), (UINT32)data.length(), format.get(), size.width, size.height, &layout));
    DWRITE_TEXT_METRICS metrics;
    XAML_RETURN_IF_FAILED(layout->GetMetrics(&metrics));
    switch (font.halign)
    {
    case xaml_halignment_center:
        region.x -= metrics.width / 2;
        break;
    case xaml_halignment_right:
        region.x -= metrics.width;
        break;
    default:
        break;
    }
    switch (font.valign)
    {
    case xaml_valignment_center:
        region.y -= metrics.height / 2;
        break;
    case xaml_valignment_bottom:
        region.y -= metrics.height;
        break;
    default:
        break;
    }
    region.width = metrics.width;
    region.height = metrics.height;
    *pvalue = region;
    if (playout) XAML_RETURN_IF_FAILED(layout.copy_to(playout));
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_string(xaml_brush* brush, xaml_drawing_font const& font, xaml_point const& p, xaml_string* str) noexcept
{
    xaml_rectangle region;
    wil::com_ptr_nothrow<IDWriteTextLayout> layout;
    XAML_RETURN_IF_FAILED(measure_string_impl(target.get(), dwrite.get(), font, p, str, &region, &layout));
    CHECK_SIZE(region);
    wil::com_ptr_nothrow<ID2D1Brush> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), brush, region, &b));
    target->DrawTextLayout(D2D1::Point2F((FLOAT)region.x, (FLOAT)region.y), layout.get(), b.get());
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::measure_string(xaml_drawing_font const& font, xaml_point const& p, xaml_string* str, xaml_rectangle* pvalue) noexcept
{
    return measure_string_impl(target.get(), dwrite.get(), font, p, str, pvalue, nullptr);
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
            wil::com_ptr_nothrow<ID2D1RenderTarget> ctx_target;
            XAML_RETURN_IF_FAILED(target.query_to(&ctx_target));
            xaml_ptr<xaml_drawing_context> dc;
            XAML_RETURN_IF_FAILED(xaml_object_new<xaml_drawing_context_impl>(&dc, ctx_target, d2d, dwrite));
            XAML_RETURN_IF_FAILED(m_redraw->invoke(m_outer_this, dc));
            XAML_RETURN_IF_FAILED(target->EndDraw());
        }
        *presult = TRUE;
        return XAML_S_OK;
    }
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
        XAML_RETURN_IF_FAILED(m_mouse_down->invoke(m_outer_this, (xaml_mouse_button)((msg.Msg - WM_LBUTTONDOWN) / 3)));
        *presult = 0;
        return XAML_S_OK;
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
        XAML_RETURN_IF_FAILED(m_mouse_up->invoke(m_outer_this, (xaml_mouse_button)((msg.Msg - WM_LBUTTONUP) / 3)));
        *presult = 0;
        return XAML_S_OK;
    case WM_MOUSEMOVE:
    {
        auto real_loc = xaml_from_native(POINT{ GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) });
        XAML_RETURN_IF_FAILED(m_mouse_move->invoke(m_outer_this, real_loc * USER_DEFAULT_SCREEN_DPI / XamlGetDpiForWindow(m_handle)));
        *presult = 0;
        return XAML_S_OK;
    }
    }
    return XAML_E_NOTIMPL;
}

xaml_result XAML_CALL xaml_canvas_internal::draw(xaml_rectangle const& region) noexcept
{
    xaml_ptr<xaml_element_base> parent;
    XAML_RETURN_IF_FAILED(get_parent(&parent));
    if (parent)
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
            params.parent = parent;
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

xaml_result XAML_CALL xaml_canvas_internal::invalidate(xaml_rectangle const* prect) noexcept
{
    if (prect)
    {
        RECT r = xaml_to_native<RECT>(*prect * XamlGetDpiForWindow(m_handle) / USER_DEFAULT_SCREEN_DPI);
        XAML_RETURN_IF_WIN32_BOOL_FALSE(InvalidateRect(m_handle, &r, FALSE));
    }
    else
    {
        XAML_RETURN_IF_WIN32_BOOL_FALSE(InvalidateRect(m_handle, nullptr, FALSE));
    }
    return XAML_S_OK;
}
