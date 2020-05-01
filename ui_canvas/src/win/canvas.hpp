#ifndef XAML_UI_CONTROLS_NATIVE_CANVAS_HPP
#define XAML_UI_CONTROLS_NATIVE_CANVAS_HPP

#include <Windows.h>
#include <xaml/ui/controls/canvas.h>

struct native_drawing_context
{
    virtual xaml_result draw_arc(xaml_drawing_pen const& pen, xaml_rectangle const& region, double start_angle, double end_angle) noexcept = 0;
    virtual xaml_result fill_pie(xaml_drawing_brush const& brush, xaml_rectangle const& region, double start_angle, double end_angle) noexcept = 0;
    virtual xaml_result draw_ellipse(xaml_drawing_pen const& pen, xaml_rectangle const& region) noexcept = 0;
    virtual xaml_result fill_ellipse(xaml_drawing_brush const& brush, xaml_rectangle const& region) noexcept = 0;
    virtual xaml_result draw_line(xaml_drawing_pen const& pen, xaml_point const& startp, xaml_point const& endp) noexcept = 0;
    virtual xaml_result draw_rect(xaml_drawing_pen const& pen, xaml_rectangle const& rect) noexcept = 0;
    virtual xaml_result fill_rect(xaml_drawing_brush const& brush, xaml_rectangle const& rect) noexcept = 0;
    virtual xaml_result draw_round_rect(xaml_drawing_pen const& pen, xaml_rectangle const& rect, xaml_size const& round) noexcept = 0;
    virtual xaml_result fill_round_rect(xaml_drawing_brush const& brush, xaml_rectangle const& rect, xaml_size const& round) noexcept = 0;
    virtual xaml_result draw_string(xaml_drawing_brush const& brush, xaml_drawing_font const& font, xaml_point const& p, xaml_std_string_view_t str) noexcept = 0;
};

struct native_canvas
{
    virtual xaml_result create(HWND wnd) noexcept = 0;
    virtual xaml_result begin_paint(HWND wnd, xaml_size const& real, std::function<void(xaml_drawing_context*)> const& paint_func) noexcept = 0;
};

#endif // !XAML_UI_CONTROLS_NATIVE_CANVAS_HPP
