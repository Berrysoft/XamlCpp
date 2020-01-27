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

    void drawing_context::draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle)
    {
        m_handle->DrawArc(pen.get_handle(), to_native<RectF>(region), (REAL)start_angle, (REAL)end_angle);
    }

    void drawing_context::fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle)
    {
        m_handle->FillPie(brush.get_handle(), to_native<RectF>(region), (REAL)start_angle, (REAL)end_angle);
    }

    void drawing_context::draw_ellipse(drawing_pen const& pen, rectangle const& region)
    {
        m_handle->DrawEllipse(pen.get_handle(), to_native<RectF>(region));
    }

    void drawing_context::fill_ellipse(drawing_brush const& brush, rectangle const& region)
    {
        m_handle->FillEllipse(brush.get_handle(), to_native<RectF>(region));
    }

    void drawing_context::draw_line(drawing_pen const& pen, point startp, point endp)
    {
        m_handle->DrawLine(pen.get_handle(), to_native<PointF>(startp), to_native<PointF>(endp));
    }

    void drawing_context::draw_rect(drawing_pen const& pen, rectangle const& rect)
    {
        m_handle->DrawRectangle(pen.get_handle(), to_native<RectF>(rect));
    }

    void drawing_context::fill_rect(drawing_brush const& brush, rectangle const& rect)
    {
        m_handle->FillRectangle(brush.get_handle(), to_native<RectF>(rect));
    }

    void drawing_context::draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str)
    {
        m_handle->DrawString(str.data(), (INT)str.length(), font.get_handle(), to_native<PointF>(p), brush.get_handle());
    }

    canvas::canvas() : common_control() {}

    canvas::~canvas() {}

    optional<LRESULT> canvas::__wnd_proc(window_message const& msg)
    {
        if (get_handle() && msg.hWnd == get_handle())
        {
            switch (msg.Msg)
            {
            case WM_PAINT:
            {
                rectangle region = m_real_region;
                THROW_IF_WIN32_BOOL_FALSE(Rectangle(m_store_dc.get(), -1, -1, (int)region.width + 1, (int)region.height + 1));
                Graphics g{ m_store_dc.get() };
                g.SetPageUnit(UnitPixel);
                drawing_context dc{ &g };
                m_redraw(*this, dc);
                if (auto wnd = __get_window(get_handle()))
                {
                    wnd->__copy_hdc(m_real_region, m_store_dc.get());
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
            auto wnd_dc = wil::GetDC(get_handle());
            m_store_dc.reset(CreateCompatibleDC(wnd_dc.get()));
            wil::unique_hbitmap bitmap{ CreateCompatibleBitmap(wnd_dc.get(), (int)m_real_region.width, (int)m_real_region.height) };
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
