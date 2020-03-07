#include <Windows.h>
#include <cmath>
#include <stdexcept>
#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/controls/canvas.hpp>
#include <xaml/ui/controls/native_canvas.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_drawing.hpp>
#include <xaml/ui/native_window.hpp>
#include <xaml/ui/window.hpp>

using namespace std;
using namespace Gdiplus;

namespace xaml
{
    static char const* gdiplus_error_message[] = {
        "OK.",
        "Generic error.",
        "Invalid parameter.",
        "Out of memory.",
        "Object busy."
        "Insufficient buffer.",
        "Not implemented.",
        "Win32 error.",
        "Wrong state.",
        "Aborted.",
        "File not found.",
        "Value overflow.",
        "Access denied.",
        "Unknown image format.",
        "Font family not found.",
        "Font style not found.",
        "Not TrueType font.",
        "Unsupported GDI+ version.",
        "GDI+ not initialized.",
        "Property not found.",
        "Property not supported.",
        "Profile not found."
    };

    struct gdiplus_error : logic_error
    {
        gdiplus_error(Gdiplus::Status status) : logic_error(gdiplus_error_message[status]) {}
        ~gdiplus_error() override {}
    };

    static inline void check_status(Gdiplus::Status status)
    {
        if (status)
        {
            throw gdiplus_error{ status };
        }
    }

    constexpr REAL get_WIDTH(double width, double dpi)
    {
        return (REAL)(width * dpi / 96.0);
    }

    static inline Pen get_Pen(drawing_pen const& pen, double dpi)
    {
        return Pen(Color((int)pen.stroke), get_WIDTH(pen.width, dpi));
    }

    static inline SolidBrush get_Brush(drawing_brush const& brush)
    {
        return SolidBrush(Color((int)brush.fill));
    }

    static constexpr INT get_font_style(bool italic, bool bold)
    {
        return (italic ? Gdiplus::FontStyleItalic : Gdiplus::FontStyleRegular) | (bold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular);
    }

    static inline Font get_Font(drawing_font const& font, double dpi)
    {
        return Font(font.font_family.c_str(), get_WIDTH(font.size, dpi), get_font_style(font.italic, font.bold), UnitPixel);
    }

    static inline RectF get_RectF(rectangle const& rect, double dpi)
    {
        return to_native<RectF>(rect * dpi / 96.0);
    }

    static inline PointF get_PointF(point p, double dpi)
    {
        return to_native<PointF>(p * dpi / 96.0);
    }

    static inline SizeF get_SizeF(size s, double dpi)
    {
        return to_native<SizeF>(s * dpi / 96.0);
    }

    void drawing_context::draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle)
    {
        auto p = get_Pen(pen, __get_dpi());
        check_status(m_handle->handle->DrawArc(&p, get_RectF(region, __get_dpi()), (REAL)(start_angle / M_PI * 180), (REAL)((end_angle - start_angle) / M_PI * 180)));
    }

    void drawing_context::fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle)
    {
        auto b = get_Brush(brush);
        check_status(m_handle->handle->FillPie(&b, get_RectF(region, __get_dpi()), (REAL)(start_angle / M_PI * 180), (REAL)((end_angle - start_angle) / M_PI * 180)));
    }

    void drawing_context::draw_ellipse(drawing_pen const& pen, rectangle const& region)
    {
        auto p = get_Pen(pen, __get_dpi());
        check_status(m_handle->handle->DrawEllipse(&p, get_RectF(region, __get_dpi())));
    }

    void drawing_context::fill_ellipse(drawing_brush const& brush, rectangle const& region)
    {
        auto b = get_Brush(brush);
        check_status(m_handle->handle->FillEllipse(&b, get_RectF(region, __get_dpi())));
    }

    void drawing_context::draw_line(drawing_pen const& pen, point startp, point endp)
    {
        auto p = get_Pen(pen, __get_dpi());
        check_status(m_handle->handle->DrawLine(&p, get_PointF(startp, __get_dpi()), get_PointF(endp, __get_dpi())));
    }

    void drawing_context::draw_rect(drawing_pen const& pen, rectangle const& rect)
    {
        auto p = get_Pen(pen, __get_dpi());
        check_status(m_handle->handle->DrawRectangle(&p, get_RectF(rect, __get_dpi())));
    }

    void drawing_context::fill_rect(drawing_brush const& brush, rectangle const& rect)
    {
        auto b = get_Brush(brush);
        check_status(m_handle->handle->FillRectangle(&b, get_RectF(rect, __get_dpi())));
    }

    static unique_ptr<GraphicsPath> RoundedRect(RectF bounds, SizeF size)
    {
        PointF ori_loc;
        bounds.GetLocation(&ori_loc);
        RectF arc{ ori_loc, { size.Width * 2, size.Height * 2 } };
        auto path = make_unique<GraphicsPath>();

        if (size.Width == 0 && size.Height == 0)
        {
            path->AddRectangle(bounds);
            return path;
        }

        // top left arc
        path->AddArc(arc, 180, 90);

        // top right arc
        arc.X = bounds.Width + bounds.X - size.Width * 2;
        path->AddArc(arc, 270, 90);

        // bottom right arc
        arc.Y = bounds.Height + bounds.Y - size.Height * 2;
        path->AddArc(arc, 0, 90);

        // bottom left arc
        arc.X = bounds.X;
        path->AddArc(arc, 90, 90);

        path->CloseFigure();
        return path;
    }

    void drawing_context::draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round)
    {
        auto p = get_Pen(pen, __get_dpi());
        auto path = RoundedRect(get_RectF(rect, __get_dpi()), get_SizeF(round, __get_dpi()));
        check_status(m_handle->handle->DrawPath(&p, path.get()));
    }

    void drawing_context::fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round)
    {
        auto b = get_Brush(brush);
        auto path = RoundedRect(get_RectF(rect, __get_dpi()), get_SizeF(round, __get_dpi()));
        check_status(m_handle->handle->FillPath(&b, path.get()));
    }

    void drawing_context::draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str)
    {
        auto b = get_Brush(brush);
        auto f = get_Font(font, __get_dpi());
        check_status(m_handle->handle->DrawString(str.data(), (INT)str.length(), &f, get_PointF(p, __get_dpi()), &b));
    }

    canvas::canvas() : control()
    {
        set_canvas(make_shared<native_canvas>());
        GdiplusStartupInput gdiplusStartupInput{};
        check_status(GdiplusStartup(&get_canvas()->token, &gdiplusStartupInput, NULL));
    }

    canvas::~canvas()
    {
        GdiplusShutdown(get_canvas()->token);
    }

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
                THROW_IF_WIN32_BOOL_FALSE(Rectangle(get_canvas()->store_dc.get(), -1, -1, (int)region.width + 1, (int)region.height + 1));
                Graphics g{ get_canvas()->store_dc.get() };
                check_status(g.SetPageUnit(UnitPixel));
                native_drawing_context ctx{};
                ctx.handle = &g;
                drawing_context dc{ &ctx };
                dc.__set_dpi((double)udpi);
                m_redraw(*this, dc);
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
