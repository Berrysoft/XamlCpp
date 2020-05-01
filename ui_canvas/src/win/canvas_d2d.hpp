#ifndef XAML_UI_CANVAS_D2D1_HPP
#define XAML_UI_CANVAS_D2D1_HPP

#include <d2d1.h>
#include <dwrite.h>
#include <wil/com.h>
#include <wil/resource.h>
#include <win/canvas.hpp>
#include <xaml/ui/native_drawing.hpp>

template <typename Factory>
HRESULT DWriteCreateFactory(DWRITE_FACTORY_TYPE factoryType, Factory** factory) noexcept
{
    return DWriteCreateFactory(factoryType, __uuidof(Factory), reinterpret_cast<IUnknown**>(factory));
}

struct drawing_context_d2d : native_drawing_context
{
    wil::com_ptr<ID2D1RenderTarget> target{ nullptr };
    wil::com_ptr<ID2D1Factory> d2d{ nullptr };
    wil::com_ptr<IDWriteFactory> dwrite{ nullptr };

    xaml_result draw_arc(xaml_drawing_pen const& pen, xaml_rectangle const& region, double start_angle, double end_angle) noexcept override;
    xaml_result fill_pie(xaml_drawing_brush const& brush, xaml_rectangle const& region, double start_angle, double end_angle) noexcept override;
    xaml_result draw_ellipse(xaml_drawing_pen const& pen, xaml_rectangle const& region) noexcept override;
    xaml_result fill_ellipse(xaml_drawing_brush const& brush, xaml_rectangle const& region) noexcept override;
    xaml_result draw_line(xaml_drawing_pen const& pen, xaml_point const& startp, xaml_point const& endp) noexcept override;
    xaml_result draw_rect(xaml_drawing_pen const& pen, xaml_rectangle const& rect) noexcept override;
    xaml_result fill_rect(xaml_drawing_brush const& brush, xaml_rectangle const& rect) noexcept override;
    xaml_result draw_round_rect(xaml_drawing_pen const& pen, xaml_rectangle const& rect, xaml_size const& round) noexcept override;
    xaml_result fill_round_rect(xaml_drawing_brush const& brush, xaml_rectangle const& rect, xaml_size const& round) noexcept override;
    xaml_result draw_string(xaml_drawing_brush const& brush, xaml_drawing_font const& font, xaml_point const& p, xaml_std_string_view_t str) noexcept override;
};

class canvas_d2d : public native_canvas
{
private:
    wil::com_ptr<ID2D1HwndRenderTarget> target{ nullptr };
    wil::com_ptr<ID2D1Factory> d2d{ nullptr };
    wil::com_ptr<IDWriteFactory> dwrite{ nullptr };

public:
    canvas_d2d() noexcept;

    xaml_result create(HWND wnd) noexcept override;
    xaml_result begin_paint(HWND wnd, xaml_size const& real, std::function<void(xaml_drawing_context*)> const& paint_func) noexcept override;
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
    return { (double)r.left, (double)r.top, (double)(r.right - r.left), (double)(r.bottom - r.top) };
}
template <>
inline D2D1_RECT_F xaml_to_native<D2D1_RECT_F, xaml_rectangle>(xaml_rectangle const& r) noexcept
{
    return { (float)r.x, (float)r.y, (float)(r.x + r.width), (float)(r.y + r.height) };
}

#endif // !XAML_UI_CANVAS_D2D1_HPP
