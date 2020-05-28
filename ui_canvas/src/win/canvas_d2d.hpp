#ifndef XAML_UI_CANVAS_D2D1_HPP
#define XAML_UI_CANVAS_D2D1_HPP

#include <d2d1.h>
#include <dwrite.h>
#include <shared/canvas.hpp>
#include <wil/com.h>
#include <wil/resource.h>
#include <win/canvas.hpp>
#include <xaml/object.h>
#include <xaml/ui/controls/canvas.h>
#include <xaml/ui/drawing_conv.hpp>

template <typename Factory>
HRESULT DWriteCreateFactory(DWRITE_FACTORY_TYPE factoryType, Factory** factory) noexcept
{
    return DWriteCreateFactory(factoryType, __uuidof(Factory), reinterpret_cast<IUnknown**>(factory));
}

struct xaml_drawing_context_d2d_impl : xaml_implement<xaml_drawing_context_d2d_impl, xaml_drawing_context, xaml_object>
{
    wil::com_ptr_t<ID2D1RenderTarget, wil::err_returncode_policy> target{ nullptr };
    wil::com_ptr_t<ID2D1Factory, wil::err_returncode_policy> d2d{ nullptr };
    wil::com_ptr_t<IDWriteFactory, wil::err_returncode_policy> dwrite{ nullptr };

    xaml_drawing_context_d2d_impl(wil::com_ptr_t<ID2D1RenderTarget, wil::err_returncode_policy> const& target, wil::com_ptr_t<ID2D1Factory, wil::err_returncode_policy> const& d2d, wil::com_ptr_t<IDWriteFactory, wil::err_returncode_policy> const& dwrite) noexcept
        : target(target), d2d(d2d), dwrite(dwrite) {}

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

struct xaml_canvas_d2d_internal : xaml_win32_canvas_internal
{
    wil::com_ptr_t<ID2D1HwndRenderTarget, wil::err_returncode_policy> target{ nullptr };
    wil::com_ptr_t<ID2D1Factory, wil::err_returncode_policy> d2d{ nullptr };
    wil::com_ptr_t<IDWriteFactory, wil::err_returncode_policy> dwrite{ nullptr };

    xaml_result XAML_CALL draw_impl() noexcept override;
    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const&, LRESULT*) noexcept override;

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_canvas_d2d_impl : xaml_win32_canvas_implement<xaml_canvas_d2d_impl, xaml_canvas_d2d_internal>
{
};

inline xaml_size xaml_from_native(D2D1_SIZE_F const& s) noexcept
{
    return { (double)s.width, (double)s.height };
}
template <>
inline D2D1_SIZE_F xaml_to_native<D2D1_SIZE_F, xaml_size>(xaml_size const& s) noexcept
{
    return { (float)s.width, (float)s.height };
}

inline xaml_point xaml_from_native(D2D1_POINT_2F const& p) noexcept
{
    return { (double)p.x, (double)p.y };
}
template <>
inline D2D1_POINT_2F xaml_to_native<D2D1_POINT_2F, xaml_point>(xaml_point const& p) noexcept
{
    return { (float)p.x, (float)p.y };
}

inline xaml_rectangle xaml_from_native(D2D1_RECT_F const& r) noexcept
{
    return { (double)r.left, (double)r.top, (double)r.right - (double)r.left, (double)r.bottom - (double)r.top };
}
template <>
inline D2D1_RECT_F xaml_to_native<D2D1_RECT_F, xaml_rectangle>(xaml_rectangle const& r) noexcept
{
    return { (float)r.x, (float)r.y, (float)(r.x + r.width), (float)(r.y + r.height) };
}

#endif // !XAML_UI_CANVAS_D2D1_HPP
