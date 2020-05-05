#ifndef XAML_UI_CANVAS_SHARED_CANVAS_HPP
#define XAML_UI_CANVAS_SHARED_CANVAS_HPP

#include <shared/control.hpp>
#include <xaml/ui/controls/canvas.h>

#ifdef XAML_UI_GTK3
#include <cairo.h>
#endif // XAML_UI_GTK3

struct xaml_drawing_context_impl : xaml_implement<xaml_drawing_context_impl, xaml_drawing_context, xaml_object>
{
#ifdef XAML_UI_GTK3
    cairo_t* m_handle;

    xaml_drawing_context_impl(cairo_t* handle) noexcept : m_handle(handle) {}
#endif // XAML_UI_GTK3

    xaml_result XAML_CALL draw_arc(xaml_drawing_pen const& pen, xaml_rectangle const& region, double start_angle, double end_angle) noexcept override;
    xaml_result XAML_CALL fill_pie(xaml_drawing_brush const& brush, xaml_rectangle const& region, double start_angle, double end_angle) noexcept override;
    xaml_result XAML_CALL draw_ellipse(xaml_drawing_pen const& pen, xaml_rectangle const& region) noexcept override;
    xaml_result XAML_CALL fill_ellipse(xaml_drawing_brush const& brush, xaml_rectangle const& region) noexcept override;
    xaml_result XAML_CALL draw_line(xaml_drawing_pen const& pen, xaml_point const& startp, xaml_point const& endp) noexcept override;
    xaml_result XAML_CALL draw_rect(xaml_drawing_pen const& pen, xaml_rectangle const& rect) noexcept override;
    xaml_result XAML_CALL fill_rect(xaml_drawing_brush const& brush, xaml_rectangle const& rect) noexcept override;
    xaml_result XAML_CALL draw_round_rect(xaml_drawing_pen const& pen, xaml_rectangle const& rect, xaml_size const& round) noexcept override;
    xaml_result XAML_CALL fill_round_rect(xaml_drawing_brush const& brush, xaml_rectangle const& rect, xaml_size const& round) noexcept override;
    xaml_result XAML_CALL draw_string(xaml_drawing_brush const& brush, xaml_drawing_font const& font, xaml_point const& p, xaml_string* str) noexcept override;
};

struct xaml_canvas_internal : xaml_control_internal
{
    XAML_EVENT_IMPL(redraw)

    xaml_result XAML_CALL init() noexcept override;
};

template <typename T, typename Internal, typename... Base>
struct xaml_canvas_implement : xaml_control_implement<T, Internal, Base..., xaml_canvas>
{
    XAML_EVENT_INTERNAL_IMPL(redraw)
};

#endif // !XAML_UI_CANVAS_SHARED_CANVAS_HPP
