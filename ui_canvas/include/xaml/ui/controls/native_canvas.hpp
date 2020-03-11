#ifndef XAML_UI_CONTROLS_NATIVE_CANVAS_HPP
#define XAML_UI_CONTROLS_NATIVE_CANVAS_HPP

#include <xaml/ui/controls/canvas.hpp>
#include <xaml/ui/window.hpp>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#elif defined(XAML_UI_GTK3)
#include <cairo.h>
#endif // XAML_UI_WINDOWS

namespace xaml
{
    struct native_drawing_context
    {
#ifdef XAML_UI_WINDOWS
        virtual ~native_drawing_context()
        {
        }

        virtual void draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle) = 0;
        virtual void fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle) = 0;
        virtual void draw_ellipse(drawing_pen const& pen, rectangle const& region) = 0;
        virtual void fill_ellipse(drawing_brush const& brush, rectangle const& region) = 0;
        virtual void draw_line(drawing_pen const& pen, point startp, point endp) = 0;
        virtual void draw_rect(drawing_pen const& pen, rectangle const& rect) = 0;
        virtual void fill_rect(drawing_brush const& brush, rectangle const& rect) = 0;
        virtual void draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round) = 0;
        virtual void fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round) = 0;
        virtual void draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str) = 0;
#elif defined(XAML_UI_GTK3)
        cairo_t* handle;
#endif // XAML_UI_WINDOWS
    };

#ifdef XAML_UI_WINDOWS
    struct native_canvas
    {
        virtual ~native_canvas() {}
        virtual bool create(HWND wnd, size real) = 0;
        virtual void begin_paint(HWND wnd, HDC dc, size real, std::function<void(drawing_context&)> paint_func) = 0;
    };
#endif // XAML_UI_WINDOWS

} // namespace xaml

#endif // !XAML_UI_CONTROLS_NATIVE_CANVAS_HPP
