#include <cairo.h>
#include <cmath>
#include <xaml/ui/controls/canvas.hpp>

namespace xaml
{
    drawing_context::drawing_context(native_handle_type handle) : m_handle(handle)
    {
    }

    void drawing_context::set_pen(drawing_pen const& pen)
    {
        cairo_set_source_rgba(m_handle, pen.get_color().r / 255.0, pen.get_color().g / 255.0, pen.get_color().b / 255.0, pen.get_color().a / 255.0);
        cairo_set_line_width(m_handle, pen.get_width());
    }

    void drawing_context::set_brush(drawing_brush const& brush)
    {
        cairo_set_source_rgba(m_handle, brush.get_color().r / 255.0, brush.get_color().g / 255.0, brush.get_color().b / 255.0, brush.get_color().a / 255.0);
    }

    void drawing_context::path_arc(rectangle const& region, double start_angle, double end_angle)
    {
        cairo_matrix_t save_matrix;
        cairo_get_matrix(m_handle, &save_matrix);
        cairo_scale(m_handle, 1, region.height / region.width);
        cairo_new_path(m_handle);
        cairo_arc(m_handle, region.x + region.width / 2, region.y + region.height / 2, region.width / 2, start_angle, end_angle);
        cairo_set_matrix(m_handle, &save_matrix);
    }

    void drawing_context::draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle)
    {
        path_arc(region, start_angle, end_angle);
        set_pen(pen);
        cairo_stroke(m_handle);
    }

    void drawing_context::fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle)
    {
        path_arc(region, start_angle, end_angle);
        set_brush(brush);
        cairo_fill(m_handle);
    }

    void drawing_context::draw_ellipse(drawing_pen const& pen, rectangle const& region)
    {
        draw_arc(pen, region, 0, 2 * M_PI);
    }

    void drawing_context::fill_ellipse(drawing_brush const& brush, const rectangle& region)
    {
        fill_pie(brush, region, 0, 2 * M_PI);
    }

    void drawing_context::draw_line(drawing_pen const& pen, point startp, point endp)
    {
        cairo_new_path(m_handle);
        cairo_move_to(m_handle, startp.x, startp.y);
        cairo_line_to(m_handle, endp.x, endp.y);
        set_pen(pen);
        cairo_stroke(m_handle);
    }

    void drawing_context::path_rect(rectangle const& rect)
    {
        cairo_new_path(m_handle);
        cairo_rectangle(m_handle, rect.x, rect.y, rect.width, rect.height);
    }

    void drawing_context::draw_rect(drawing_pen const& pen, rectangle const& rect)
    {
        path_rect(rect);
        set_pen(pen);
        cairo_stroke(m_handle);
    }

    void drawing_context::fill_rect(drawing_brush const& brush, const rectangle& rect)
    {
        path_rect(rect);
        set_brush(brush);
        cairo_stroke(m_handle);
    }

    void drawing_context::path_round_rect(rectangle const& rect, size round)
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
    }

    void drawing_context::draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round)
    {
        path_round_rect(rect, round);
        set_pen(pen);
        cairo_stroke(m_handle);
    }

    void drawing_context::fill_round_rect(drawing_brush const& brush, const rectangle& rect, size round)
    {
        path_round_rect(rect, round);
        set_brush(brush);
        cairo_stroke(m_handle);
    }

    void drawing_context::draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str)
    {
        set_brush(brush);
        cairo_select_font_face(m_handle, font.get_font_family().data(), font.get_italic() ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL, font.get_bold() ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(m_handle, font.get_size());
        cairo_move_to(m_handle, p.x, p.y);
        cairo_show_text(m_handle, str.data());
    }

    void canvas::__draw(const rectangle& region)
    {
        if (!get_handle())
        {
            set_handle(gtk_drawing_area_new());
            g_signal_connect(G_OBJECT(get_handle()), "draw", G_CALLBACK(canvas::on_draw), this);
        }
        m_real_region = region - get_margin();
        gtk_widget_set_size_request(get_handle(), (gint)get_width(), (gint)get_height());
    }

    gboolean canvas::on_draw(GtkWidget* widget, cairo_t* cr, gpointer data)
    {
        canvas* self = (canvas*)data;
        drawing_context cv{ cr };
        self->m_redraw(*self, cv);
        return FALSE;
    }
} // namespace xaml
