#include <cmath>
#include <tuple>
#include <windowsx.h>
#include <xaml/ui/controls/canvas.hpp>
#include <xaml/ui/controls/native_canvas.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_window.hpp>
#include <xaml/ui/window.hpp>

using namespace std;

namespace xaml
{
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

    void drawing_context::draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle)
    {
        wil::com_ptr<ID2D1PathGeometry> geo;
        THROW_IF_FAILED(m_handle->d2->CreatePathGeometry(&geo));
        wil::com_ptr<ID2D1GeometrySink> sink;
        THROW_IF_FAILED(geo->Open(&sink));
        auto [radius, centerp, startp, endp] = get_arc(region, start_angle, end_angle);
        sink->BeginFigure(get_POINT(startp, __get_dpi()), D2D1_FIGURE_BEGIN_HOLLOW);
        sink->AddArc(D2D1::ArcSegment(get_POINT(endp, __get_dpi()), get_SIZE(radius, __get_dpi()), 0, D2D1_SWEEP_DIRECTION_CLOCKWISE, ((end_angle - start_angle) > M_PI) ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL));
        sink->EndFigure(D2D1_FIGURE_END_OPEN);
        THROW_IF_FAILED(sink->Close());
        auto b = get_Brush(m_handle->handle.get(), pen.stroke);
        m_handle->handle->DrawGeometry(geo.get(), b.get(), get_WIDTH(pen.width, __get_dpi()));
    }

    void drawing_context::fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle)
    {
        wil::com_ptr<ID2D1PathGeometry> geo;
        THROW_IF_FAILED(m_handle->d2->CreatePathGeometry(&geo));
        wil::com_ptr<ID2D1GeometrySink> sink;
        THROW_IF_FAILED(geo->Open(&sink));
        auto [radius, centerp, startp, endp] = get_arc(region, start_angle, end_angle);
        sink->BeginFigure(get_POINT(centerp, __get_dpi()), D2D1_FIGURE_BEGIN_FILLED);
        sink->AddLine(get_POINT(startp, __get_dpi()));
        sink->AddArc(D2D1::ArcSegment(get_POINT(endp, __get_dpi()), get_SIZE(radius, __get_dpi()), 0, D2D1_SWEEP_DIRECTION_CLOCKWISE, ((end_angle - start_angle) > M_PI) ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL));
        sink->EndFigure(D2D1_FIGURE_END_CLOSED);
        THROW_IF_FAILED(sink->Close());
        auto b = get_Brush(m_handle->handle.get(), brush.fill);
        m_handle->handle->FillGeometry(geo.get(), b.get());
    }

    void drawing_context::draw_ellipse(drawing_pen const& pen, rectangle const& region)
    {
        auto e = get_ELLIPSE(region, __get_dpi());
        auto b = get_Brush(m_handle->handle.get(), pen.stroke);
        m_handle->handle->DrawEllipse(e, b.get(), get_WIDTH(pen.width, __get_dpi()));
    }

    void drawing_context::fill_ellipse(drawing_brush const& brush, rectangle const& region)
    {
        auto e = get_ELLIPSE(region, __get_dpi());
        auto b = get_Brush(m_handle->handle.get(), brush.fill);
        m_handle->handle->FillEllipse(e, b.get());
    }

    void drawing_context::draw_line(drawing_pen const& pen, point startp, point endp)
    {
        auto b = get_Brush(m_handle->handle.get(), pen.stroke);
        m_handle->handle->DrawLine(get_POINT(startp, __get_dpi()), get_POINT(endp, __get_dpi()), b.get(), get_WIDTH(pen.width, __get_dpi()));
    }

    void drawing_context::draw_rect(drawing_pen const& pen, rectangle const& rect)
    {
        auto r = get_RECT(rect, __get_dpi());
        auto b = get_Brush(m_handle->handle.get(), pen.stroke);
        m_handle->handle->DrawRectangle(r, b.get(), get_WIDTH(pen.width, __get_dpi()));
    }

    void drawing_context::fill_rect(drawing_brush const& brush, rectangle const& rect)
    {
        auto r = get_RECT(rect, __get_dpi());
        auto b = get_Brush(m_handle->handle.get(), brush.fill);
        m_handle->handle->FillRectangle(r, b.get());
    }

    void drawing_context::draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round)
    {
        auto r = get_RECT(rect, __get_dpi());
        auto s = get_SIZE(round, __get_dpi());
        auto b = get_Brush(m_handle->handle.get(), pen.stroke);
        m_handle->handle->DrawRoundedRectangle(D2D1::RoundedRect(r, s.width, s.height), b.get(), get_WIDTH(pen.width, __get_dpi()));
    }

    void drawing_context::fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round)
    {
        auto r = get_RECT(rect, __get_dpi());
        auto s = get_SIZE(round, __get_dpi());
        auto b = get_Brush(m_handle->handle.get(), brush.fill);
        m_handle->handle->FillRoundedRectangle(D2D1::RoundedRect(r, s.width, s.height), b.get());
    }

    void drawing_context::draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str)
    {
        auto b = get_Brush(m_handle->handle.get(), brush.fill);
        wil::com_ptr<IDWriteTextFormat> format;
        THROW_IF_FAILED(m_handle->dwrite->CreateTextFormat(
            font.font_family.c_str(), nullptr,
            font.bold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
            font.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL, get_WIDTH(font.size, __get_dpi()), L"", &format));
        auto size = m_handle->handle->GetSize();
        auto region = get_RECT({ p.x, p.y, p.x, p.y },__get_dpi());
        region.right += size.width;
        region.bottom += size.height;
        m_handle->handle->DrawText(str.data(), (UINT32)str.length(), format.get(), region, b.get());
    }

    canvas::canvas() : control() {}

    canvas::~canvas() {}

    optional<std::intptr_t> canvas::__wnd_proc(window_message const& msg)
    {
        if (get_handle() && msg.hWnd == get_handle()->handle)
        {
            switch (msg.Msg)
            {
            case WM_PAINT:
            {
                if (auto wnd = __get_window(get_handle()->handle))
                {
                    UINT udpi = GetDpiForWindow(get_handle()->handle);
                    rectangle region = m_real_region * udpi / 96.0;
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
                    THROW_IF_FAILED(get_canvas()->factory->CreateDCRenderTarget(&prop, &target));
                    RECT rc_region = to_native<RECT>(region);
                    THROW_IF_FAILED(target->BindDC(wnd->get_window()->store_dc.get(), &rc_region));
                    target->BeginDraw();
                    target->Clear(D2D1::ColorF(D2D1::ColorF::White));
                    native_drawing_context ctx{};
                    ctx.d2 = get_canvas()->factory.copy<ID2D1Factory>();
                    ctx.handle = target.query<ID2D1RenderTarget>();
                    THROW_IF_FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), ctx.dwrite.put_unknown()));
                    drawing_context dc{ &ctx };
                    dc.__set_dpi((double)udpi);
                    m_redraw(*this, dc);
                    THROW_IF_FAILED(target->EndDraw());
                }
                break;
            }
            }
        }
        return nullopt;
    }

    void canvas::__draw(rectangle const& region)
    {
        if (auto sparent = get_parent().lock())
        {
            set_handle(sparent->get_handle());
            if (!get_canvas())
            {
                auto cv = make_shared<native_canvas>();
                THROW_IF_FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &cv->factory));
                set_canvas(cv);
            }
            auto real = region - get_margin();
            m_real_region = real;
        }
    }
} // namespace xaml
