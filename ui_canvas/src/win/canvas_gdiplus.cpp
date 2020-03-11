#include <stdexcept>
#include <win/canvas_gdiplus.hpp>
#include <windowsx.h>
#include <xaml/ui/native_control.hpp>

using namespace std;
using namespace Gdiplus;

namespace xaml
{
    drawing_context_gdiplus::~drawing_context_gdiplus() {}

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

    void drawing_context_gdiplus::draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle)
    {
        auto p = get_Pen(pen, dpi);
        check_status(handle->DrawArc(&p, get_RectF(region, dpi), (REAL)(start_angle / M_PI * 180), (REAL)((end_angle - start_angle) / M_PI * 180)));
    }

    void drawing_context_gdiplus::fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle)
    {
        auto b = get_Brush(brush);
        check_status(handle->FillPie(&b, get_RectF(region, dpi), (REAL)(start_angle / M_PI * 180), (REAL)((end_angle - start_angle) / M_PI * 180)));
    }

    void drawing_context_gdiplus::draw_ellipse(drawing_pen const& pen, rectangle const& region)
    {
        auto p = get_Pen(pen, dpi);
        check_status(handle->DrawEllipse(&p, get_RectF(region, dpi)));
    }

    void drawing_context_gdiplus::fill_ellipse(drawing_brush const& brush, rectangle const& region)
    {
        auto b = get_Brush(brush);
        check_status(handle->FillEllipse(&b, get_RectF(region, dpi)));
    }

    void drawing_context_gdiplus::draw_line(drawing_pen const& pen, point startp, point endp)
    {
        auto p = get_Pen(pen, dpi);
        check_status(handle->DrawLine(&p, get_PointF(startp, dpi), get_PointF(endp, dpi)));
    }

    void drawing_context_gdiplus::draw_rect(drawing_pen const& pen, rectangle const& rect)
    {
        auto p = get_Pen(pen, dpi);
        check_status(handle->DrawRectangle(&p, get_RectF(rect, dpi)));
    }

    void drawing_context_gdiplus::fill_rect(drawing_brush const& brush, rectangle const& rect)
    {
        auto b = get_Brush(brush);
        check_status(handle->FillRectangle(&b, get_RectF(rect, dpi)));
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

    void drawing_context_gdiplus::draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round)
    {
        auto p = get_Pen(pen, dpi);
        auto path = RoundedRect(get_RectF(rect, dpi), get_SizeF(round, dpi));
        check_status(handle->DrawPath(&p, path.get()));
    }

    void drawing_context_gdiplus::fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round)
    {
        auto b = get_Brush(brush);
        auto path = RoundedRect(get_RectF(rect, dpi), get_SizeF(round, dpi));
        check_status(handle->FillPath(&b, path.get()));
    }

    void drawing_context_gdiplus::draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str)
    {
        auto b = get_Brush(brush);
        auto f = get_Font(font, dpi);
        check_status(handle->DrawString(str.data(), (INT)str.length(), &f, get_PointF(p, dpi), &b));
    }

    canvas_gdiplus::canvas_gdiplus()
    {
        GdiplusStartupInput gdiplusStartupInput{};
        check_status(GdiplusStartup(&token, &gdiplusStartupInput, NULL));
    }

    canvas_gdiplus::~canvas_gdiplus()
    {
        GdiplusShutdown(token);
    }

    bool canvas_gdiplus::create(HWND wnd, size real)
    {
        return true;
    }

    void canvas_gdiplus::begin_paint(HWND wnd, HDC hdc, size real, function<void(drawing_context&)> paint_func)
    {
        UINT dpi = GetDpiForWindow(wnd);
        size region = real * dpi / 96.0;
        THROW_IF_WIN32_BOOL_FALSE(Rectangle(hdc, -1, -1, (int)region.width + 2, (int)region.height + 2));
        Graphics g{ hdc };
        check_status(g.SetPageUnit(UnitPixel));
        drawing_context_gdiplus ctx{};
        ctx.handle = &g;
        ctx.dpi = (double)dpi;
        drawing_context dc{ &ctx };
        paint_func(dc);
    }
} // namespace xaml
