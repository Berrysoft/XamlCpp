#include <win/canvas_gdiplus.hpp>
#include <xaml/ui/controls/canvas.hpp>
#include <xaml/ui/controls/native_canvas.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/window.hpp>

#include <CommCtrl.h>

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
        switch (msg.Msg)
        {
        case WM_DRAWITEM:
        {
            DRAWITEMSTRUCT* ds = (DRAWITEMSTRUCT*)msg.lParam;
            if (ds->hwndItem == get_handle()->handle)
            {
                m_canvas->begin_paint(ds->hwndItem, get_size(), [this](shared_ptr<drawing_context> dc) { m_redraw(static_pointer_cast<canvas>(shared_from_this()), dc); });
            }
            return TRUE;
        }
        }
        return nullopt;
    }

    void canvas::__draw(rectangle const& region)
    {
        if (auto sparent = get_parent().lock())
        {
            if (!get_handle())
            {
                window_create_params params = {};
                params.class_name = WC_STATIC;
                params.style = WS_CHILD | WS_VISIBLE | SS_OWNERDRAW;
                params.x = 0;
                params.y = 0;
                params.width = 100;
                params.height = 50;
                params.parent = sparent.get();
                this->__create(params);
                SetParent(get_handle()->handle, sparent->get_handle()->handle);
                draw_visible();
            }
            __set_rect(region);
            if (!get_canvas())
            {
#ifdef XAML_UI_CANVAS_DIRECT2D
                set_canvas(make_shared<canvas_d2d>());
                if (!get_canvas()->create(get_handle()->handle))
                {
                    set_canvas(make_shared<canvas_gdiplus>());
                }
#else
                set_canvas(make_shared<canvas_gdiplus>());
#endif // XAML_UI_CANVAS_DIRECT2D
            }
            get_canvas()->create(get_handle()->handle);
        }
    }
} // namespace xaml
