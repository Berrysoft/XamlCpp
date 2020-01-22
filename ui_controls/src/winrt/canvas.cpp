#include "winrt/Microsoft.Graphics.Canvas.Brushes.h"
#include "winrt/Microsoft.Graphics.Canvas.Geometry.h"
#include "winrt/Microsoft.Graphics.Canvas.Text.h"
#include "winrt/Microsoft.Graphics.Canvas.UI.Xaml.h"
#include "winrt/Windows.UI.Text.h"
#include <winrt/drawing.hpp>
#include <xaml/ui/canvas.hpp>

using namespace winrt;
using namespace Windows::UI;
using namespace Windows::UI::Text;
using namespace Microsoft::Graphics::Canvas::UI::Xaml;
using namespace Microsoft::Graphics::Canvas::Brushes;
using namespace Microsoft::Graphics::Canvas::Geometry;
using namespace Microsoft::Graphics::Canvas::Text;

namespace xaml
{
    drawing_context::drawing_context(native_handle_type handle) : m_handle(handle)
    {
    }

    static Color get_Color(color c) { return ColorHelper::FromArgb(c.a, c.r, c.g, c.b); }

    static CanvasTextFormat get_Format(drawing_font const& font)
    {
        CanvasTextFormat format{};
        format.FontFamily(font.get_font_family());
        format.FontSize((float)font.get_size());
        if (font.get_italic()) format.FontStyle(FontStyle::Italic);
        if (font.get_bold()) format.FontWeight(FontWeights::Bold());
        return format;
    }

    void drawing_context::draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle)
    {
        CanvasPathBuilder builder{ m_handle };
        builder.BeginFigure((float)(region.x + region.width / 2 + region.width / 2 * cos(start_angle)), (float)(region.y + region.height / 2 + region.height / 2 * sin(start_angle)));
        builder.AddArc({ (float)(region.x + region.width / 2), (float)(region.y + region.height / 2) }, (float)region.width / 2, (float)region.height / 2, (float)start_angle, (float)(end_angle - start_angle));
        builder.EndFigure(CanvasFigureLoop::Open);
        CanvasGeometry geometry = CanvasGeometry::CreatePath(builder);
        m_handle.DrawGeometry(geometry, get_Color(pen.get_color()), (float)pen.get_width());
    }

    void drawing_context::fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle)
    {
        CanvasPathBuilder builder{ m_handle };
        builder.BeginFigure((float)(region.x + region.width / 2), (float)(region.y + region.height / 2));
        builder.AddArc({ (float)(region.x + region.width / 2), (float)(region.y + region.height / 2) }, (float)region.width / 2, (float)region.height / 2, (float)start_angle, (float)(end_angle - start_angle));
        builder.EndFigure(CanvasFigureLoop::Closed);
        CanvasGeometry geometry = CanvasGeometry::CreatePath(builder);
        m_handle.FillGeometry(geometry, get_Color(brush.get_color()));
    }

    void drawing_context::draw_ellipse(drawing_pen const& pen, rectangle const& region)
    {
        m_handle.DrawEllipse((float)(region.x + region.width / 2), (float)(region.y + region.height / 2), (float)region.width / 2.0f, (float)region.height / 2.0f, get_Color(pen.get_color()), (float)pen.get_width());
    }

    void drawing_context::fill_ellipse(drawing_brush const& brush, rectangle const& region)
    {
        m_handle.FillEllipse((float)(region.x + region.width / 2), (float)(region.y + region.height / 2), (float)region.width / 2.0f, (float)region.height / 2.0f, get_Color(brush.get_color()));
    }

    void drawing_context::draw_line(drawing_pen const& pen, point startp, point endp)
    {
        m_handle.DrawLine(get_float2(startp), get_float2(endp), get_Color(pen.get_color()), (float)pen.get_width());
    }

    void drawing_context::draw_rect(drawing_pen const& pen, rectangle const& rect)
    {
        m_handle.DrawRectangle(get_Rect(rect), get_Color(pen.get_color()), (float)pen.get_width());
    }

    void drawing_context::fill_rect(drawing_brush const& brush, rectangle const& rect)
    {
        m_handle.FillRectangle(get_Rect(rect), get_Color(brush.get_color()));
    }

    void drawing_context::draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round)
    {
        m_handle.DrawRoundedRectangle(get_Rect(rect), (float)round.width / 2, (float)round.height / 2, get_Color(pen.get_color()), (float)pen.get_width());
    }

    void drawing_context::fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round)
    {
        m_handle.FillRoundedRectangle(get_Rect(rect), (float)round.width / 2, (float)round.height / 2, get_Color(brush.get_color()));
    }

    void drawing_context::draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str)
    {
        m_handle.DrawText(str, get_float2(p), get_Color(brush.get_color()), get_Format(font));
    }

    canvas::canvas() : common_control()
    {
    }
    canvas::~canvas() {}

    void canvas::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            CanvasControl cv{};
            cv.Draw([this](CanvasControl const& cv, CanvasDrawEventArgs const& args) -> void {
                auto real_size = xaml::get_size(cv.ActualSize());
                auto real_pos = cv.ActualOffset();
                m_real_region = { real_pos.x, real_pos.y, real_size.width, real_size.height };
                drawing_context dc{ args.DrawingSession() };
                m_redraw(*this, dc);
            });
            set_handle(cv);
        }
        CanvasControl cv = get_handle().as<CanvasControl>();
        cv.Margin(get_Thickness(get_margin()));
    }
} // namespace xaml
