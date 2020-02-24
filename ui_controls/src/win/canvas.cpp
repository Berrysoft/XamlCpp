#include <Windows.h>
#include <cmath>
#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/controls/canvas.hpp>
#include <xaml/ui/window.hpp>

using namespace std;
using namespace Gdiplus;

namespace xaml
{
    drawing_brush::drawing_brush(color c) : m_object(Color((int32_t)c))
    {
    }

    color drawing_brush::get_color() const
    {
        Color c;
        m_object.GetColor(&c);
        return color::from_argb(c.GetValue());
    }

    void drawing_brush::set_color(color value)
    {
        m_object.SetColor(Color((int32_t)value));
    }

    drawing_pen::drawing_pen(color c, double width) : m_object(Color((int32_t)c), (float)width)
    {
    }

    color drawing_pen::get_color() const
    {
        Color c;
        m_object.GetColor(&c);
        return color::from_argb(c.GetValue());
    }

    void drawing_pen::set_color(color value)
    {
        m_object.SetColor(Color((int32_t)value));
    }

    double drawing_pen::get_width() const
    {
        return m_object.GetWidth();
    }

    void drawing_pen::set_width(double value)
    {
        m_object.SetWidth((float)value);
    }

    constexpr INT get_font_style(bool italic, bool bold)
    {
        return (italic ? Gdiplus::FontStyleItalic : Gdiplus::FontStyleRegular) | (bold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular);
    }

    drawing_font::drawing_font(string_view_t family, double size, bool italic, bool bold) : m_object(family.data(), (REAL)size, get_font_style(italic, bold))
    {
    }

    double drawing_font::get_size() const
    {
        return m_object.GetSize();
    }

    bool drawing_font::get_italic() const
    {
        return m_object.GetStyle() & FontStyleItalic;
    }

    bool drawing_font::get_bold() const
    {
        return m_object.GetStyle() & FontStyleBold;
    }

    drawing_context::drawing_context(native_handle_type handle) : m_handle(handle)
    {
    }

    static inline RectF get_RectF(rectangle const& rect, double dpi)
    {
        return to_native<RectF>(rect * dpi / 96.0);
    }

    static inline PointF get_PointF(point p, double dpi)
    {
        return to_native<PointF>(p * dpi / 96.0);
    }

    void drawing_context::draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle)
    {
        m_handle->DrawArc(pen.get_handle(), get_RectF(region, __get_dpi()), (REAL)start_angle, (REAL)end_angle);
    }

    void drawing_context::fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle)
    {
        m_handle->FillPie(brush.get_handle(), get_RectF(region, __get_dpi()), (REAL)start_angle, (REAL)end_angle);
    }

    void drawing_context::draw_ellipse(drawing_pen const& pen, rectangle const& region)
    {
        m_handle->DrawEllipse(pen.get_handle(), get_RectF(region, __get_dpi()));
    }

    void drawing_context::fill_ellipse(drawing_brush const& brush, rectangle const& region)
    {
        m_handle->FillEllipse(brush.get_handle(), get_RectF(region, __get_dpi()));
    }

    void drawing_context::draw_line(drawing_pen const& pen, point startp, point endp)
    {
        m_handle->DrawLine(pen.get_handle(), get_PointF(startp, __get_dpi()), get_PointF(endp, __get_dpi()));
    }

    void drawing_context::draw_rect(drawing_pen const& pen, rectangle const& rect)
    {
        m_handle->DrawRectangle(pen.get_handle(), get_RectF(rect, __get_dpi()));
    }

    void drawing_context::fill_rect(drawing_brush const& brush, rectangle const& rect)
    {
        m_handle->FillRectangle(brush.get_handle(), get_RectF(rect, __get_dpi()));
    }

    void drawing_context::draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str)
    {
        m_handle->DrawString(str.data(), (INT)str.length(), font.get_handle(), get_PointF(p, __get_dpi()), brush.get_handle());
    }

    optional<LRESULT> canvas::__wnd_proc(window_message const& msg)
    {
        if (get_handle() && msg.hWnd == get_handle())
        {
            switch (msg.Msg)
            {
            case WM_PAINT:
            {
                UINT udpi = GetDpiForWindow(get_handle());
                rectangle region = m_real_region * udpi / 96.0;
                THROW_IF_WIN32_BOOL_FALSE(Rectangle(m_store_dc.get(), -1, -1, (int)region.width + 1, (int)region.height + 1));
                Graphics g{ m_store_dc.get() };
                g.SetPageUnit(UnitPixel);
                drawing_context dc{ &g };
                dc.__set_dpi((double)udpi);
                m_redraw(*this, dc);
                if (auto wnd = __get_window(get_handle()))
                {
                    wnd->__copy_hdc(region, m_store_dc.get());
                }
                break;
            }
            }
        }
        return nullopt;
    }

    void canvas::__draw(rectangle const& region)
    {
        set_handle(get_parent()->get_handle());
        auto real = region - get_margin();
        if (m_real_region != real)
        {
            m_real_region = real;
            UINT udpi = GetDpiForWindow(get_handle());
            rectangle real_real = m_real_region * udpi / 96.0;
            auto wnd_dc = wil::GetDC(get_handle());
            m_store_dc.reset(CreateCompatibleDC(wnd_dc.get()));
            wil::unique_hbitmap bitmap{ CreateCompatibleBitmap(wnd_dc.get(), (int)real_real.width, (int)real_real.height) };
            wil::unique_hbitmap ori_bitmap{ SelectBitmap(m_store_dc.get(), bitmap.release()) };
        }
    }

    struct gdiplus_init_guard
    {
        ULONG_PTR token;
        gdiplus_init_guard()
        {
            GdiplusStartupInput gdiplusStartupInput{};
            GdiplusStartup(&token, &gdiplusStartupInput, NULL);
        }
        ~gdiplus_init_guard() { GdiplusShutdown(token); }
    };

    static gdiplus_init_guard init_guard{};
} // namespace xaml
