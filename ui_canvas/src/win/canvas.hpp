#ifndef XAML_UI_CONTROLS_NATIVE_CANVAS_HPP
#define XAML_UI_CONTROLS_NATIVE_CANVAS_HPP

#include <Windows.h>
#include <shared/canvas.hpp>
#include <xaml/ui/controls/canvas.h>

#include <CommCtrl.h>

struct xaml_win32_canvas_internal : xaml_canvas_internal
{
    virtual xaml_result XAML_CALL draw_impl() noexcept = 0;

    xaml_result XAML_CALL draw(xaml_rectangle const& region) noexcept override;
};

template <typename T, typename Internal, typename... Base>
struct xaml_win32_canvas_implement : xaml_canvas_implement<T, Internal, Base...>
{
};

#endif // !XAML_UI_CONTROLS_NATIVE_CANVAS_HPP
