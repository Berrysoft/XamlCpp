#include <xaml/ui/controls/canvas.hpp>
#include <xaml/ui/controls/native_canvas.hpp>

using namespace std;

namespace xaml
{
#ifndef XAML_UI_WINDOWS
    canvas::canvas() : control()
    {
    }

    canvas::~canvas() {}
#endif // !XAML_UI_WINDOWS

    drawing_context::drawing_context(native_handle_type handle) : m_handle(handle)
    {
    }

#ifndef XAML_UI_COCOA
    void drawing_context::draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle)
    {
        m_handle->draw_arc(pen, region, start_angle, end_angle);
    }

    void drawing_context::fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle)
    {
        m_handle->fill_pie(brush, region, start_angle, end_angle);
    }

    void drawing_context::draw_ellipse(drawing_pen const& pen, rectangle const& region)
    {
        m_handle->draw_ellipse(pen, region);
    }

    void drawing_context::fill_ellipse(drawing_brush const& brush, rectangle const& region)
    {
        m_handle->fill_ellipse(brush, region);
    }

    void drawing_context::draw_line(drawing_pen const& pen, point startp, point endp)
    {
        m_handle->draw_line(pen, startp, endp);
    }

    void drawing_context::draw_rect(drawing_pen const& pen, rectangle const& rect)
    {
        m_handle->draw_rect(pen, rect);
    }

    void drawing_context::fill_rect(drawing_brush const& brush, rectangle const& rect)
    {
        m_handle->fill_rect(brush, rect);
    }

    void drawing_context::draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round)
    {
        m_handle->draw_round_rect(pen, rect, round);
    }

    void drawing_context::fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round)
    {
        m_handle->fill_round_rect(brush, rect, round);
    }

    void drawing_context::draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str)
    {
        m_handle->draw_string(brush, font, p, str);
    }
#endif // !XAML_UI_COCOA
} // namespace xaml
