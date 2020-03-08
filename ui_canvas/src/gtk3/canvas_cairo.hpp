#ifndef XAML_UI_CANVAS_CAIRO_HPP
#define XAML_UI_CANVAS_CAIRO_HPP

#include <gtk/gtk.h>
#include <xaml/ui/controls/native_canvas.hpp>

namespace xaml
{
    struct drawing_context_cairo : native_drawing_context
    {
        cairo_t* handle;

        ~drawing_context_cairo() override;

        void set_pen(drawing_pen const& pen);
        void set_brush(drawing_brush const& brush);
        void path_arc(rectangle const& region, double start_angle, double end_angle);
        void path_rect(rectangle const& rect);
        void path_round_rect(rectangle const& rect, size round);

        void draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle) override;
        void fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle) override;
        void draw_ellipse(drawing_pen const& pen, rectangle const& region) override;
        void fill_ellipse(drawing_brush const& brush, rectangle const& region) override;
        void draw_line(drawing_pen const& pen, point startp, point endp) override;
        void draw_rect(drawing_pen const& pen, rectangle const& rect) override;
        void fill_rect(drawing_brush const& brush, rectangle const& rect) override;
        void draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round) override;
        void fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round) override;
        void draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str) override;
    };
} // namespace xaml

#endif // !XAML_UI_CANVAS_CAIRO_HPP
