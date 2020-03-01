#include <cairo.h>
#include <cmath>
#include <xaml/ui/controls/canvas.hpp>
#include <xaml/ui/controls/native_canvas.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_drawing.hpp>

using namespace std;

namespace xaml
{
    void drawing_context::set_pen(drawing_pen const& pen)
    {
        cairo_set_source_rgba(m_handle->handle, pen.stroke.r / 255.0, pen.stroke.g / 255.0, pen.stroke.b / 255.0, pen.stroke.a / 255.0);
        cairo_set_line_width(m_handle->handle, pen.width);
    }

    void drawing_context::set_brush(drawing_brush const& brush)
    {
        cairo_set_source_rgba(m_handle->handle, brush.fill.r / 255.0, brush.fill.g / 255.0, brush.fill.b / 255.0, brush.fill.a / 255.0);
    }

    void drawing_context::path_arc(rectangle const& region, double start_angle, double end_angle)
    {
        cairo_matrix_t save_matrix;
        cairo_get_matrix(m_handle->handle, &save_matrix);
        cairo_scale(m_handle->handle, 1, region.height / region.width);
        cairo_new_path(m_handle->handle);
        cairo_arc(m_handle->handle, region.x + region.width / 2, region.y + region.height / 2, region.width / 2, start_angle, end_angle);
        cairo_set_matrix(m_handle->handle, &save_matrix);
    }

    void drawing_context::draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle)
    {
        path_arc(region, start_angle, end_angle);
        set_pen(pen);
        cairo_stroke(m_handle->handle);
    }

    void drawing_context::fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle)
    {
        path_arc(region, start_angle, end_angle);
        set_brush(brush);
        cairo_fill(m_handle->handle);
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
        cairo_new_path(m_handle->handle);
        cairo_move_to(m_handle->handle, startp.x, startp.y);
        cairo_line_to(m_handle->handle, endp.x, endp.y);
        set_pen(pen);
        cairo_stroke(m_handle->handle);
    }

    void drawing_context::path_rect(rectangle const& rect)
    {
        cairo_new_path(m_handle->handle);
        cairo_rectangle(m_handle->handle, rect.x, rect.y, rect.width, rect.height);
    }

    void drawing_context::draw_rect(drawing_pen const& pen, rectangle const& rect)
    {
        path_rect(rect);
        set_pen(pen);
        cairo_stroke(m_handle->handle);
    }

    void drawing_context::fill_rect(drawing_brush const& brush, const rectangle& rect)
    {
        path_rect(rect);
        set_brush(brush);
        cairo_stroke(m_handle->handle);
    }

    void drawing_context::path_round_rect(rectangle const& rect, size round)
    {
        cairo_matrix_t save_matrix;
        cairo_get_matrix(m_handle->handle, &save_matrix);
        cairo_scale(m_handle->handle, 1, round.height / round.width);
        cairo_new_sub_path(m_handle->handle);
        cairo_arc(m_handle->handle, rect.x + rect.width - round.width / 2, rect.y + round.height / 2, round.width / 2, -M_PI / 2, 0);
        cairo_arc(m_handle->handle, rect.x + rect.width - round.width / 2, rect.y + rect.height - round.height / 2, round.width / 2, 0, M_PI / 2);
        cairo_arc(m_handle->handle, rect.x + round.width / 2, rect.y + rect.height - round.height / 2, round.width / 2, M_PI / 2, M_PI);
        cairo_arc(m_handle->handle, rect.x + round.width / 2, rect.y + round.height / 2, round.width / 2, M_PI, M_PI / 2 * 3);
        cairo_close_path(m_handle->handle);
        cairo_set_matrix(m_handle->handle, &save_matrix);
    }

    void drawing_context::draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round)
    {
        path_round_rect(rect, round);
        set_pen(pen);
        cairo_stroke(m_handle->handle);
    }

    void drawing_context::fill_round_rect(drawing_brush const& brush, const rectangle& rect, size round)
    {
        path_round_rect(rect, round);
        set_brush(brush);
        cairo_stroke(m_handle->handle);
    }

    void drawing_context::draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str)
    {
        set_brush(brush);
        cairo_select_font_face(m_handle->handle, font.font_family.c_str(), font.italic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL, font.bold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(m_handle->handle, font.size);
        cairo_move_to(m_handle->handle, p.x, p.y);
        cairo_show_text(m_handle->handle, str.data());
    }

    void canvas::__draw(const rectangle& region)
    {
        if (!get_handle())
        {
            auto h = make_shared<native_control>();
            h->handle = gtk_drawing_area_new();
            set_handle(h);
            g_signal_connect(G_OBJECT(get_handle()->handle), "draw", G_CALLBACK(canvas::on_draw), this);
        }
        m_real_region = region - get_margin();
        __set_rect(region);
    }

    int canvas::on_draw(void* widget, void* cr, void* data)
    {
        canvas* self = (canvas*)data;
        native_drawing_context ctx{ (cairo_t*)cr };
        drawing_context cv{ &ctx };
        self->m_redraw(*self, cv);
        return FALSE;
    }
} // namespace xaml
