#include <cmath>
#include <xaml/ui/canvas.hpp>

namespace xaml
{
    void drawing_context::draw_arc(rectangle const& region, double start_angle, double end_angle)
    {
        cairo_matrix_t save_matrix;
        cairo_get_matrix(m_handle, &save_matrix);
        cairo_scale(m_handle, 1, region.height / region.width);
        cairo_new_path(m_handle);
        cairo_arc(m_handle, region.x + region.width / 2, region.y + region.height / 2, region.width / 2, start_angle, end_angle);
        cairo_set_matrix(m_handle, &save_matrix);
        cairo_stroke(m_handle);
    }

    void drawing_context::draw_ellipse(rectangle const& region)
    {
        draw_arc(region, 0, 2 * M_PI);
    }

    void drawing_context::draw_line(point startp, point endp)
    {
        cairo_new_path(m_handle);
        cairo_move_to(m_handle, startp.x, startp.y);
        cairo_line_to(m_handle, endp.x, endp.y);
        cairo_stroke(m_handle);
    }

    void drawing_context::draw_rect(rectangle const& rect)
    {
        cairo_new_path(m_handle);
        cairo_rectangle(m_handle, rect.x, rect.y, rect.width, rect.height);
        cairo_stroke(m_handle);
    }

    void drawing_context::draw_round_rect(rectangle const& rect, size round)
    {
        cairo_matrix_t save_matrix;
        cairo_get_matrix(m_handle, &save_matrix);
        cairo_scale(m_handle, 1, round.height / round.width);
        cairo_new_sub_path(m_handle);
        cairo_arc(m_handle, rect.x + rect.width - round.width / 2, rect.y + round.height / 2, round.width / 2, -M_PI / 2, 0);
        cairo_arc(m_handle, rect.x + rect.width - round.width / 2, rect.y + rect.height - round.height / 2, round.width / 2, 0, M_PI / 2);
        cairo_arc(m_handle, rect.x + round.width / 2, rect.y + rect.height - round.height / 2, round.width / 2, M_PI / 2, M_PI);
        cairo_arc(m_handle, rect.x + round.width / 2, rect.y + round.height / 2, round.width / 2, M_PI, M_PI / 2 * 3);
        cairo_close_path(m_handle);
        cairo_set_matrix(m_handle, &save_matrix);
        cairo_stroke(m_handle);
    }

    void drawing_context::draw_string(point p, string_view_t str)
    {
        cairo_move_to(m_handle, p.x, p.y);
        cairo_show_text(m_handle, str.data());
    }
} // namespace xaml
