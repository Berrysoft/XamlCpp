#ifdef XAML_UI_CANVAS_DIRECT2D

#include <tuple>
#include <win/canvas_d2d.hpp>
#include <xaml/ui/native_window.hpp>

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

    static D2D1_ELLIPSE get_ELLIPSE(rectangle const& region, double dpi)
    {
        return D2D1::Ellipse(D2D1::Point2F((FLOAT)((region.x + region.width / 2) * dpi / 96.0), (FLOAT)((region.y + region.height / 2) * dpi / 96.0)), (FLOAT)(region.width / 2 * dpi / 96.0), (FLOAT)(region.height / 2 * dpi / 96.0));
    }

    static D2D1_RECT_F get_RECT(rectangle const& rect, double dpi)
    {
        return to_native<D2D1_RECT_F>(rect * dpi / 96.0);
    }

    static D2D1_POINT_2F get_POINT(point p, double dpi)
    {
        return to_native<D2D1_POINT_2F>(p * dpi / 96.0);
    }

    static D2D1_SIZE_F get_SIZE(size s, double dpi)
    {
        return to_native<D2D1_SIZE_F>(s * dpi / 96.0);
    }

    constexpr FLOAT get_WIDTH(double width, double dpi)
    {
        return (FLOAT)(width * dpi / 96.0);
    }

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
        wil::com_ptr<ID2D1PathGeometry> geo;
        THROW_IF_FAILED(d2d->CreatePathGeometry(&geo));
        wil::com_ptr<ID2D1GeometrySink> sink;
        THROW_IF_FAILED(geo->Open(&sink));
        auto [radius, centerp, startp, endp] = get_arc(region, start_angle, end_angle);
        sink->BeginFigure(get_POINT(startp, dpi), D2D1_FIGURE_BEGIN_HOLLOW);
        sink->AddArc(D2D1::ArcSegment(get_POINT(endp, dpi), get_SIZE(radius, dpi), 0, D2D1_SWEEP_DIRECTION_CLOCKWISE, ((end_angle - start_angle) > M_PI) ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL));
        sink->EndFigure(D2D1_FIGURE_END_OPEN);
        THROW_IF_FAILED(sink->Close());
        auto b = get_Brush(target.get(), pen.stroke);
        target->DrawGeometry(geo.get(), b.get(), get_WIDTH(pen.width, dpi));
    }

    void drawing_context_d2d::fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle)
    {
        wil::com_ptr<ID2D1PathGeometry> geo;
        THROW_IF_FAILED(d2d->CreatePathGeometry(&geo));
        wil::com_ptr<ID2D1GeometrySink> sink;
        THROW_IF_FAILED(geo->Open(&sink));
        auto [radius, centerp, startp, endp] = get_arc(region, start_angle, end_angle);
        sink->BeginFigure(get_POINT(centerp, dpi), D2D1_FIGURE_BEGIN_FILLED);
        sink->AddLine(get_POINT(startp, dpi));
        sink->AddArc(D2D1::ArcSegment(get_POINT(endp, dpi), get_SIZE(radius, dpi), 0, D2D1_SWEEP_DIRECTION_CLOCKWISE, ((end_angle - start_angle) > M_PI) ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL));
        sink->EndFigure(D2D1_FIGURE_END_CLOSED);
        THROW_IF_FAILED(sink->Close());
        auto b = get_Brush(target.get(), brush.fill);
        target->FillGeometry(geo.get(), b.get());
    }

    void drawing_context_d2d::draw_ellipse(drawing_pen const& pen, rectangle const& region)
    {
        auto e = get_ELLIPSE(region, dpi);
        auto b = get_Brush(target.get(), pen.stroke);
        target->DrawEllipse(e, b.get(), get_WIDTH(pen.width, dpi));
    }

    void drawing_context_d2d::fill_ellipse(drawing_brush const& brush, rectangle const& region)
    {
        auto e = get_ELLIPSE(region, dpi);
        auto b = get_Brush(target.get(), brush.fill);
        target->FillEllipse(e, b.get());
    }

    void drawing_context_d2d::draw_line(drawing_pen const& pen, point startp, point endp)
    {
        auto b = get_Brush(target.get(), pen.stroke);
        target->DrawLine(get_POINT(startp, dpi), get_POINT(endp, dpi), b.get(), get_WIDTH(pen.width, dpi));
    }

    void drawing_context_d2d::draw_rect(drawing_pen const& pen, rectangle const& rect)
    {
        auto r = get_RECT(rect, dpi);
        auto b = get_Brush(target.get(), pen.stroke);
        target->DrawRectangle(r, b.get(), get_WIDTH(pen.width, dpi));
    }

    void drawing_context_d2d::fill_rect(drawing_brush const& brush, rectangle const& rect)
    {
        auto r = get_RECT(rect, dpi);
        auto b = get_Brush(target.get(), brush.fill);
        target->FillRectangle(r, b.get());
    }

    void drawing_context_d2d::draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round)
    {
        auto r = get_RECT(rect, dpi);
        auto s = get_SIZE(round, dpi);
        auto b = get_Brush(target.get(), pen.stroke);
        target->DrawRoundedRectangle(D2D1::RoundedRect(r, s.width, s.height), b.get(), get_WIDTH(pen.width, dpi));
    }

    void drawing_context_d2d::fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round)
    {
        auto r = get_RECT(rect, dpi);
        auto s = get_SIZE(round, dpi);
        auto b = get_Brush(target.get(), brush.fill);
        target->FillRoundedRectangle(D2D1::RoundedRect(r, s.width, s.height), b.get());
    }

    void drawing_context_d2d::draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str)
    {
        auto b = get_Brush(target.get(), brush.fill);
        wil::com_ptr<IDWriteTextFormat> format;
        THROW_IF_FAILED(dwrite->CreateTextFormat(
            font.font_family.c_str(), nullptr,
            font.bold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
            font.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL, get_WIDTH(font.size, dpi), L"", &format));
        auto size = target->GetSize();
        auto region = get_RECT({ p.x, p.y, p.x, p.y }, dpi);
        region.right += size.width;
        region.bottom += size.height;
        target->DrawText(str.data(), (UINT32)str.length(), format.get(), region, b.get());
    }

    static ID2D1Factory* try_create_factory()
    {
        ID2D1Factory* factory = nullptr;
        __try
        {
            HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
            if (FAILED(hr)) factory = nullptr;
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            factory = nullptr;
        }
        return factory;
    }

    canvas_d2d::canvas_d2d() {}

    canvas_d2d::~canvas_d2d() {}

    bool canvas_d2d::create(shared_ptr<window> wnd, rectangle const& real)
    {
        return (bool)(d2d = try_create_factory());
    }

    void canvas_d2d::begin_paint(shared_ptr<window> wnd, rectangle const& real, function<void(drawing_context&)> paint_func)
    {
        double dpi = wnd->get_dpi();
        rectangle region = real * dpi / 96.0;
        wil::com_ptr<ID2D1DCRenderTarget> target;
        auto prop = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(
                DXGI_FORMAT_B8G8R8A8_UNORM,
                D2D1_ALPHA_MODE_PREMULTIPLIED),
            0,
            0,
            D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
            D2D1_FEATURE_LEVEL_DEFAULT);
        THROW_IF_FAILED(d2d->CreateDCRenderTarget(&prop, &target));
        RECT rc_region = to_native<RECT>(region);
        THROW_IF_FAILED(target->BindDC(wnd->get_window()->store_dc.get(), &rc_region));
        target->BeginDraw();
        target->Clear(D2D1::ColorF(D2D1::ColorF::White));
        drawing_context_d2d ctx{};
        ctx.d2d = d2d.copy<ID2D1Factory>();
        ctx.target = target.query<ID2D1RenderTarget>();
        ctx.dpi = dpi;
        THROW_IF_FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), ctx.dwrite.put_unknown()));
        drawing_context dc{ &ctx };
        paint_func(dc);
        THROW_IF_FAILED(target->EndDraw());
    }
} // namespace xaml

#endif // XAML_UI_CANVAS_DIRECT2D
