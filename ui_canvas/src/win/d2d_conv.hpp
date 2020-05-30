#ifndef XAML_UI_CANVAS_WIN_D2D_CONV_HPP
#define XAML_UI_CANVAS_WIN_D2D_CONV_HPP

#include <xaml/ui/drawing_conv.hpp>

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

#endif // !XAML_UI_CANVAS_WIN_D2D_CONV_HPP
