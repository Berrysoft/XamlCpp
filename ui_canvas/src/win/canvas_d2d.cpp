#ifdef XAML_UI_CANVAS_DIRECT2D

#include <tuple>
#include <wil/result_macros.h>
#include <win/canvas_d2d.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_window.hpp>
#include <xaml/ui/win/d2d.h>
#include <xaml/ui/win/dpi.h>

using namespace std;

namespace xaml
{
    drawing_context_d2d::~drawing_context_d2d() {}

    static inline D2D1::ColorF get_COLOR(color c)
    {
        return (uint32_t)c;
    }

    static wil::com_ptr<ID2D1Brush> get_Brush(ID2D1RenderTarget* target, color c)
    {
        wil::com_ptr<ID2D1SolidColorBrush> brush;
        target->CreateSolidColorBrush(get_COLOR(c), &brush);
        return brush;
    }

    static D2D1_ELLIPSE get_ELLIPSE(rectangle const& region)
    {
        return D2D1::Ellipse(D2D1::Point2F((FLOAT)(region.x + region.width / 2), (FLOAT)(region.y + region.height / 2)), (FLOAT)(region.width / 2), (FLOAT)(region.height / 2));
    }

#define CHECK_SIZE(r) \
    if ((r).width < 1 || (r).height < 1) return

    static constexpr tuple<size, point, point, point> get_arc(rectangle const& region, double start_angle, double end_angle)
    {
        size radius = { region.width / 2, region.height / 2 };
        point centerp = { region.x + radius.width, region.y + radius.height };
        point startp = centerp + point{ radius.width * cos(start_angle), radius.height * sin(start_angle) };
        point endp = centerp + point{ radius.width * cos(end_angle), radius.height * sin(end_angle) };
        return make_tuple(radius, centerp, startp, endp);
    }

    void drawing_context_d2d::draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle)
    {
        CHECK_SIZE(region);
        wil::com_ptr<ID2D1PathGeometry> geo;
        THROW_IF_FAILED(d2d->CreatePathGeometry(&geo));
        wil::com_ptr<ID2D1GeometrySink> sink;
        THROW_IF_FAILED(geo->Open(&sink));
        auto [radius, centerp, startp, endp] = get_arc(region, start_angle, end_angle);
        sink->BeginFigure(to_native<D2D1_POINT_2F>(startp), D2D1_FIGURE_BEGIN_HOLLOW);
        sink->AddArc(D2D1::ArcSegment(to_native<D2D1_POINT_2F>(endp), to_native<D2D1_SIZE_F>(radius), 0, D2D1_SWEEP_DIRECTION_CLOCKWISE, ((end_angle - start_angle) > M_PI) ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL));
        sink->EndFigure(D2D1_FIGURE_END_OPEN);
        THROW_IF_FAILED(sink->Close());
        auto b = get_Brush(target.get(), pen.stroke);
        target->DrawGeometry(geo.get(), b.get(), (FLOAT)pen.width);
    }

    void drawing_context_d2d::fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle)
    {
        CHECK_SIZE(region);
        wil::com_ptr<ID2D1PathGeometry> geo;
        THROW_IF_FAILED(d2d->CreatePathGeometry(&geo));
        wil::com_ptr<ID2D1GeometrySink> sink;
        THROW_IF_FAILED(geo->Open(&sink));
        auto [radius, centerp, startp, endp] = get_arc(region, start_angle, end_angle);
        sink->BeginFigure(to_native<D2D1_POINT_2F>(centerp), D2D1_FIGURE_BEGIN_FILLED);
        sink->AddLine(to_native<D2D1_POINT_2F>(startp));
        sink->AddArc(D2D1::ArcSegment(to_native<D2D1_POINT_2F>(endp), to_native<D2D1_SIZE_F>(radius), 0, D2D1_SWEEP_DIRECTION_CLOCKWISE, ((end_angle - start_angle) > M_PI) ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL));
        sink->EndFigure(D2D1_FIGURE_END_CLOSED);
        THROW_IF_FAILED(sink->Close());
        auto b = get_Brush(target.get(), brush.fill);
        target->FillGeometry(geo.get(), b.get());
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
        auto r = to_native<D2D1_RECT_F>(rect);
        auto s = to_native<D2D1_SIZE_F>(round);
        auto b = get_Brush(target.get(), pen.stroke);
        target->DrawRoundedRectangle(D2D1::RoundedRect(r, s.width, s.height), b.get(), (FLOAT)pen.width);
    }

    void drawing_context_d2d::fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round)
    {
        CHECK_SIZE(rect);
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
        }
        switch (font.valign)
        {
        case valignment_t::center:
            region.top -= metrics.height / 2;
            break;
        case valignment_t::bottom:
            region.top -= metrics.height;
            break;
        }
        target->DrawTextLayout(D2D1::Point2F(region.left, region.top), layout.get(), b.get());
    }

    canvas_d2d::canvas_d2d()
    {
        XamlInitializeD2DFunc();
    }

    canvas_d2d::~canvas_d2d() {}

    bool canvas_d2d::create(HWND wnd)
    {
        if (!d2d)
        {
            try
            {
                THROW_IF_FAILED(XamlD2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d));
                THROW_IF_FAILED(d2d->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(wnd), &target));
                THROW_IF_FAILED(XamlDWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, &dwrite));
                return true;
            }
            catch (wil::ResultException const&)
            {
                return false;
            }
        }
        return false;
    }

    void canvas_d2d::begin_paint(HWND wnd, size real, function<void(shared_ptr<drawing_context>)> paint_func)
    {
        UINT dpi = XamlGetDpiForWindow(wnd);
        size region = real * dpi / 96.0;
        CHECK_SIZE(region);
        target->Resize(D2D1::SizeU((UINT32)region.width, (UINT32)region.height));
        target->BeginDraw();
        target->SetDpi((FLOAT)dpi, (FLOAT)dpi);
        target->Clear(D2D1::ColorF(D2D1::ColorF::White));
        drawing_context_d2d ctx{};
        ctx.d2d = d2d.copy<ID2D1Factory>();
        ctx.target = target.query<ID2D1RenderTarget>();
        ctx.dwrite = dwrite.copy<IDWriteFactory>();
        auto dc = make_shared<drawing_context>(&ctx);
        paint_func(dc);
        THROW_IF_FAILED(target->EndDraw());
    }
} // namespace xaml

#endif // XAML_UI_CANVAS_DIRECT2D
