#ifndef XAML_UI_CANVAS_GDIPLUS_HPP
#define XAML_UI_CANVAS_GDIPLUS_HPP

#include <Windows.h>
#include <optional>
#include <shared/canvas.hpp>
#include <wil/resource.h>
#include <win/canvas.hpp>
#include <xaml/ui/native_drawing.hpp>

#include <gdiplus.h>

struct xaml_drawing_context_gdiplus_impl : xaml_implement<xaml_drawing_context_gdiplus_impl, xaml_drawing_context, xaml_object>
{
    double dpi{ USER_DEFAULT_SCREEN_DPI };
    Gdiplus::Graphics* handle{ nullptr };

    xaml_drawing_context_gdiplus_impl(Gdiplus::Graphics* handle, double dpi) noexcept : handle(handle), dpi(dpi) {}

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

struct xaml_canvas_gdiplus_internal : xaml_win32_canvas_internal
{
    std::optional<ULONG_PTR> m_token{};

    ~xaml_canvas_gdiplus_internal();

    xaml_result XAML_CALL draw_impl() noexcept override;
    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const&, LRESULT*) noexcept override;

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_canvas_gdiplus_impl : xaml_win32_canvas_implement<xaml_canvas_gdiplus_impl, xaml_canvas_gdiplus_internal>
{
};

inline xaml_size xaml_from_native(Gdiplus::SizeF const& s) noexcept
{
    return { (double)s.Width, (double)s.Height };
}
template <>
inline Gdiplus::SizeF xaml_to_native<Gdiplus::SizeF, xaml_size>(xaml_size const& s) noexcept
{
    return { (float)s.width, (float)s.height };
}

inline xaml_point xaml_from_native(Gdiplus::PointF const& p) noexcept
{
    return { (double)p.X, (double)p.Y };
}
template <>
inline Gdiplus::PointF xaml_to_native<Gdiplus::PointF, xaml_point>(xaml_point const& p) noexcept
{
    return { (float)p.x, (float)p.y };
}

inline xaml_rectangle xaml_from_native(Gdiplus::RectF const& r) noexcept
{
    return { (double)r.X, (double)r.Y, (double)r.Width, (double)r.Height };
}
template <>
inline Gdiplus::RectF xaml_to_native<Gdiplus::RectF, xaml_rectangle>(xaml_rectangle const& r) noexcept
{
    return { (float)r.x, (float)r.y, (float)r.width, (float)r.height };
}

#endif // !XAML_UI_CANVAS_GDIPLUS_HPP
