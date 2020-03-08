#include <cairo.h>
#include <cmath>
#include <gtk3/canvas_cairo.hpp>

namespace xaml
{
    drawing_context_cairo::~drawing_context_cairo() {}

    void drawing_context_cairo::set_pen(drawing_pen const& pen)
    {
        cairo_set_source_rgba(handle, pen.stroke.r / 255.0, pen.stroke.g / 255.0, pen.stroke.b / 255.0, pen.stroke.a / 255.0);
        cairo_set_line_width(handle, pen.width);
    }

    void drawing_context_cairo::set_brush(drawing_brush const& brush)
    {
        cairo_set_source_rgba(handle, brush.fill.r / 255.0, brush.fill.g / 255.0, brush.fill.b / 255.0, brush.fill.a / 255.0);
    }

    void drawing_context_cairo::path_arc(rectangle const& region, double start_angle, double end_angle)
    {
        cairo_matrix_t save_matrix;
        cairo_get_matrix(handle, &save_matrix);
        cairo_scale(handle, 1, region.height / region.width);
        cairo_new_path(handle);
        cairo_arc(handle, region.x + region.width / 2, region.y + region.height / 2, region.width / 2, start_angle, end_angle);
        cairo_set_matrix(handle, &save_matrix);
    }

    void drawing_context_cairo::draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle)
    {
        path_arc(region, start_angle, end_angle);
        set_pen(pen);
        cairo_stroke(handle);
    }

    void drawing_context_cairo::fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle)
    {
        path_arc(region, start_angle, end_angle);
        set_brush(brush);
        cairo_fill(handle);
    }

    void drawing_context_cairo::draw_ellipse(drawing_pen const& pen, rectangle const& region)
    {
        draw_arc(pen, region, 0, 2 * M_PI);
    }

    void drawing_context_cairo::fill_ellipse(drawing_brush const& brush, const rectangle& region)
    {
        fill_pie(brush, region, 0, 2 * M_PI);
    }

    void drawing_context_cairo::draw_line(drawing_pen const& pen, point startp, point endp)
    {
        cairo_new_path(handle);
        cairo_move_to(handle, startp.x, startp.y);
        cairo_line_to(handle, endp.x, endp.y);
        set_pen(pen);
        cairo_stroke(handle);
    }

    void drawing_context_cairo::path_rect(rectangle const& rect)
    {
        cairo_new_path(handle);
        cairo_rectangle(handle, rect.x, rect.y, rect.width, rect.height);
    }

    void drawing_context_cairo::draw_rect(drawing_pen const& pen, rectangle const& rect)
    {
        path_rect(rect);
        set_pen(pen);
        cairo_stroke(handle);
    }

    void drawing_context_cairo::fill_rect(drawing_brush const& brush, const rectangle& rect)
    {
        path_rect(rect);
        set_brush(brush);
        cairo_stroke(handle);
    }

    void drawing_context_cairo::path_round_rect(rectangle const& rect, size round)
    {
        cairo_matrix_t save_matrix;
        cairo_get_matrix(handle, &save_matrix);
        cairo_scale(handle, 1, round.height / round.width);
        cairo_new_sub_path(handle);
        cairo_arc(handle, rect.x + rect.width - round.width / 2, rect.y + round.height / 2, round.width / 2, -M_PI / 2, 0);
        cairo_arc(handle, rect.x + rect.width - round.width / 2, rect.y + rect.height - round.height / 2, round.width / 2, 0, M_PI / 2);
        cairo_arc(handle, rect.x + round.width / 2, rect.y + rect.height - round.height / 2, round.width / 2, M_PI / 2, M_PI);
        cairo_arc(handle, rect.x + round.width / 2, rect.y + round.height / 2, round.width / 2, M_PI, M_PI / 2 * 3);
        cairo_close_path(handle);
        cairo_set_matrix(handle, &save_matrix);
    }

    void drawing_context_cairo::draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round)
    {
        path_round_rect(rect, round);
        set_pen(pen);
        cairo_stroke(handle);
    }

    void drawing_context_cairo::fill_round_rect(drawing_brush const& brush, const rectangle& rect, size round)
    {
        path_round_rect(rect, round);
        set_brush(brush);
        cairo_stroke(handle);
    }

    void drawing_context_cairo::draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str)
    {
        set_brush(brush);
        cairo_select_font_face(handle, font.font_family.c_str(), font.italic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL, font.bold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(handle, font.size);
        cairo_move_to(handle, p.x, p.y);
        cairo_show_text(handle, str.data());
    }
} // namespace xaml
