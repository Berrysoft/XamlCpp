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

    constexpr FLOAT get_WIDTH(double width, double dpi)
    {
        return (FLOAT)(width * dpi / 96.0);
    }

    void drawing_context::draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle)
    {
    }

    void drawing_context::fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle)
    {
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
        m_handle->handle->DrawText(str.data(), (UINT32)str.length(), format.get(), get_RECT({ p.x, p.y, p.x + size.width, p.y + size.height }, __get_dpi()), b.get());
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
                RECT rc_region = { 0, 0, (LONG)region.width, (LONG)region.height };
                THROW_IF_FAILED(target->BindDC(get_canvas()->store_dc.get(), &rc_region));
                target->BeginDraw();
                target->Clear(D2D1::ColorF(D2D1::ColorF::White));
                native_drawing_context ctx{};
                ctx.handle = target.query<ID2D1RenderTarget>();
                THROW_IF_FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&ctx.dwrite));
                drawing_context dc{ &ctx };
                dc.__set_dpi((double)udpi);
                m_redraw(*this, dc);
                THROW_IF_FAILED(target->EndDraw());
                if (auto wnd = __get_window(get_handle()->handle))
                {
                    wnd->__copy_hdc(region, get_canvas()->store_dc.get());
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
            if (m_real_region != real)
            {
                m_real_region = real;
                UINT udpi = GetDpiForWindow(get_handle()->handle);
                rectangle real_real = m_real_region * udpi / 96.0;
                auto wnd_dc = wil::GetDC(get_handle()->handle);
                get_canvas()->store_dc.reset(CreateCompatibleDC(wnd_dc.get()));
                wil::unique_hbitmap bitmap{ CreateCompatibleBitmap(wnd_dc.get(), (int)real_real.width, (int)real_real.height) };
                DeleteBitmap(SelectBitmap(get_canvas()->store_dc.get(), bitmap.release()));
            }
        }
    }
} // namespace xaml
