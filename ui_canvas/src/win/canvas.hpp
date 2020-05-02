#ifndef XAML_UI_CONTROLS_NATIVE_CANVAS_HPP
#define XAML_UI_CONTROLS_NATIVE_CANVAS_HPP

#include <Windows.h>
#include <shared/canvas.hpp>
#include <xaml/ui/controls/canvas.h>

#include <CommCtrl.h>

template <typename T, typename... Base>
struct xaml_win32_canvas_implement : xaml_canvas_implement<T, Base...>
{
    virtual xaml_result XAML_CALL draw_impl() noexcept = 0;

    xaml_result XAML_CALL draw(xaml_rectangle const& region) noexcept override
    {
        if (m_parent)
        {
            if (!m_handle)
            {
                xaml_win32_window_create_params params = {};
                params.class_name = WC_STATIC;
                params.style = WS_CHILD | WS_VISIBLE | SS_OWNERDRAW;
                params.x = 0;
                params.y = 0;
                params.width = 100;
                params.height = 50;
                params.parent = m_parent;
                XAML_RETURN_IF_FAILED(create(params));
                XAML_RETURN_IF_FAILED(draw_visible());
            }
            XAML_RETURN_IF_FAILED(set_rect(region));
            XAML_RETURN_IF_FAILED(draw_impl());
        }
        return XAML_S_OK;
    }
};

#endif // !XAML_UI_CONTROLS_NATIVE_CANVAS_HPP
