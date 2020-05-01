#ifndef XAML_UI_CANVAS_SHARED_CANVAS_HPP
#define XAML_UI_CANVAS_SHARED_CANVAS_HPP

#include <shared/control.hpp>
#include <xaml/ui/controls/canvas.h>

#ifdef XAML_UI_WINDOWS
#include <memory>
#include <win/canvas.hpp>
#endif // XAML_UI_WINDOWS

struct xaml_drawing_context_impl : xaml_implement<xaml_drawing_context_impl, xaml_drawing_context, xaml_object>
{
#ifdef XAML_UI_WINDOWS
    std::unique_ptr<native_drawing_context> m_native_dc{ nullptr };
#endif // XAML_UI_WINDOWS
};

struct xaml_canvas_impl : xaml_control_implement<xaml_canvas_impl, xaml_canvas>
{
    XAML_EVENT_IMPL(redraw)

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

#ifdef XAML_UI_WINDOWS
    std::unique_ptr<native_canvas> m_native_canvas{ nullptr };

    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const&, LRESULT*) noexcept override;
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL init() noexcept override;
};

#endif // !XAML_UI_CANVAS_SHARED_CANVAS_HPP
