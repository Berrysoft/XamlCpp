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
    return brush.try_copy_to(ptr);
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

static HRESULT get_arc_geo(wil::com_ptr<ID2D1Factory> const& d2d, xaml_rectangle const& region, double start_angle, double end_angle, bool close, ID2D1PathGeometry** ptr) noexcept
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
    return geo.try_copy_to(ptr);
}

xaml_result drawing_context_d2d::draw_arc(xaml_drawing_pen const& pen, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    CHECK_SIZE(region);
    wil::com_ptr_t<ID2D1PathGeometry, wil::err_returncode_policy> geo;
    XAML_RETURN_IF_FAILED(get_arc_geo(d2d, region, start_angle, end_angle, false, &geo));
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), pen.stroke, &b));
    target->DrawGeometry(geo.get(), b.get(), (FLOAT)pen.width);
    return XAML_S_OK;
}

xaml_result drawing_context_d2d::fill_pie(xaml_drawing_brush const& brush, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    CHECK_SIZE(region);
    wil::com_ptr_t<ID2D1PathGeometry, wil::err_returncode_policy> geo;
    XAML_RETURN_IF_FAILED(get_arc_geo(d2d, region, start_angle, end_angle, false, &geo));
    wil::com_ptr_t<ID2D1Brush, wil::err_returncode_policy> b;
    XAML_RETURN_IF_FAILED(get_Brush(target.get(), brush.fill, &b));
    target->FillGeometry(geo.get(), b.get());
    return XAML_S_OK;
}

void drawing_context_d2d::draw_ellipse(drawing_pen const& pen, rectangle const& region)
{
    CHECK_SIZE(region);
    auto e = get_ELLIPSE(region);
    auto b = get_Brush(target.get(), pen.stroke);
    target->DrawEllipse(e, b.get(), (FLOAT)pen.width);
}

void drawing_context_d2d::fill_ellipse(drawing_brush const& brush, rectangle const& region)
{
    CHECK_SIZE(region);
    auto e = get_ELLIPSE(region);
    auto b = get_Brush(target.get(), brush.fill);
    target->FillEllipse(e, b.get());
}

void drawing_context_d2d::draw_line(drawing_pen const& pen, point startp, point endp)
{
    auto b = get_Brush(target.get(), pen.stroke);
    target->DrawLine(to_native<D2D1_POINT_2F>(startp), to_native<D2D1_POINT_2F>(endp), b.get(), (FLOAT)pen.width);
}

void drawing_context_d2d::draw_rect(drawing_pen const& pen, rectangle const& rect)
{
    CHECK_SIZE(rect);
    auto r = to_native<D2D1_RECT_F>(rect);
    auto b = get_Brush(target.get(), pen.stroke);
    target->DrawRectangle(r, b.get(), (FLOAT)pen.width);
}

void drawing_context_d2d::fill_rect(drawing_brush const& brush, rectangle const& rect)
{
    CHECK_SIZE(rect);
    auto r = to_native<D2D1_RECT_F>(rect);
    auto b = get_Brush(target.get(), brush.fill);
    target->FillRectangle(r, b.get());
}

void drawing_context_d2d::draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round)
{
    CHECK_SIZE(rect);
    CHECK_SIZE(round);
    auto r = to_native<D2D1_RECT_F>(rect);
    auto s = to_native<D2D1_SIZE_F>(round);
    auto b = get_Brush(target.get(), pen.stroke);
    target->DrawRoundedRectangle(D2D1::RoundedRect(r, s.width, s.height), b.get(), (FLOAT)pen.width);
}

void drawing_context_d2d::fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round)
{
    CHECK_SIZE(rect);
    CHECK_SIZE(round);
    auto r = to_native<D2D1_RECT_F>(rect);
    auto s = to_native<D2D1_SIZE_F>(round);
    auto b = get_Brush(target.get(), brush.fill);
    target->FillRoundedRectangle(D2D1::RoundedRect(r, s.width, s.height), b.get());
}

void drawing_context_d2d::draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str)
{
    auto b = get_Brush(target.get(), brush.fill);
    auto fsize = (FLOAT)font.size;
    if (fsize <= 0) return;
    wil::com_ptr<IDWriteTextFormat> format;
    THROW_IF_FAILED(dwrite->CreateTextFormat(
        font.font_family.c_str(), nullptr,
        font.bold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
        font.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL, fsize, L"", &format));
    auto size = target->GetSize();
    auto region = to_native<D2D1_RECT_F, rectangle>({ p.x, p.y, p.x, p.y });
    wil::com_ptr<IDWriteTextLayout> layout;
    THROW_IF_FAILED(dwrite->CreateTextLayout(str.data(), (UINT32)str.length(), format.get(), size.width, size.height, &layout));
    DWRITE_TEXT_METRICS metrics;
    THROW_IF_FAILED(layout->GetMetrics(&metrics));
    switch (font.halign)
    {
    case halignment_t::center:
        region.left -= metrics.width / 2;
        break;
    case halignment_t::right:
        region.left -= metrics.width;
        break;
    default:
        break;
    }
    switch (font.valign)
    {
    case valignment_t::center:
        region.top -= metrics.height / 2;
        break;
    case valignment_t::bottom:
        region.top -= metrics.height;
        break;
    default:
        break;
    }
    target->DrawTextLayout(D2D1::Point2F(region.left, region.top), layout.get(), b.get());
}

canvas_d2d::canvas_d2d() noexcept {}

xaml_result canvas_d2d::create(HWND wnd) noexcept
{
    if (!d2d)
    {
        XAML_RETURN_IF_FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d));
        XAML_RETURN_IF_FAILED(d2d->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(wnd), &target));
        XAML_RETURN_IF_FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, &dwrite));
        return XAML_S_OK;
    }
    return XAML_E_FAIL;
}

xaml_result canvas_d2d::begin_paint(HWND wnd, xaml_size const& real, function<void(xaml_drawing_context*)> const& paint_func) noexcept
{
    UINT dpi = XamlGetDpiForWindow(wnd);
    xaml_size region = real * dpi / USER_DEFAULT_SCREEN_DPI;
    CHECK_SIZE(region);
    target->Resize(D2D1::SizeU((UINT32)region.width, (UINT32)region.height));
    target->BeginDraw();
    target->SetDpi((FLOAT)dpi, (FLOAT)dpi);
    D2D1_COLOR_F background_color = XamlIsDarkModeAllowedForApp() ? D2D1::ColorF(D2D1::ColorF::Black) : D2D1::ColorF(D2D1::ColorF::White);
    target->Clear(background_color);
    drawing_context_d2d ctx{};
    ctx.d2d = d2d.copy<ID2D1Factory>();
    ctx.target = target.query<ID2D1RenderTarget>();
    ctx.dwrite = dwrite.copy<IDWriteFactory>();
    xaml_ptr<xaml_drawing_context> dc;
    XAML_RETURN_IF_FAILED(xaml_object_new<xaml_drawing_context_impl>(&dc, &ctx));
    paint_func(dc.get());
    THROW_IF_FAILED(target->EndDraw());
}

#endif // XAML_UI_CANVAS_DIRECT2D
