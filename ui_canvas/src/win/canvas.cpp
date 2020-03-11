#include <win/canvas_gdiplus.hpp>
#include <xaml/ui/controls/canvas.hpp>
#include <xaml/ui/controls/native_canvas.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_window.hpp>
#include <xaml/ui/window.hpp>

#ifdef XAML_UI_CANVAS_DIRECT2D
#include <win/canvas_d2d.hpp>
#endif // XAML_UI_CANVAS_DIRECT2D

using namespace std;

namespace xaml
{
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

    optional<std::intptr_t> canvas::__wnd_proc(window_message const& msg)
    {
        if (get_handle() && msg.hWnd == get_handle()->handle)
        {
            switch (msg.Msg)
            {
            case WM_PAINT:
            {
                PAINTSTRUCT ps;
                auto hDC = wil::BeginPaint(get_handle()->handle, &ps);
                if (auto wnd = __get_window(get_handle()->handle))
                {
                    m_canvas->begin_paint(wnd, m_real_region, [this](drawing_context& dc) { m_redraw(*this, dc); });
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
                __set_size_noevent({ real.width, real.height });
                if (auto wnd = __get_window(get_handle()->handle))
                {
                    if (!get_canvas())
                    {
#ifdef XAML_UI_CANVAS_DIRECT2D
                        set_canvas(make_shared<canvas_d2d>());
                        if (!get_canvas()->create(wnd, m_real_region))
                        {
                            set_canvas(make_shared<canvas_gdiplus>());
                        }
#else
                        set_canvas(make_shared<canvas_gdiplus>());
#endif // XAML_UI_CANVAS_DIRECT2D
                    }
                    get_canvas()->create(wnd, m_real_region);
                }
            }
        }
    }
} // namespace xaml
