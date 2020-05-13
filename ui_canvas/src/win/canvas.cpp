#include <win/canvas_gdiplus.hpp>
#include <xaml/ui/controls/canvas.h>

#ifdef XAML_UI_CANVAS_DIRECT2D
#include <win/canvas_d2d.hpp>
#endif // XAML_UI_CANVAS_DIRECT2D

using namespace std;

xaml_result XAML_CALL xaml_win32_canvas_internal::draw(xaml_rectangle const& region) noexcept
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

xaml_result XAML_CALL xaml_canvas_new(xaml_canvas** ptr) noexcept
{
#ifdef XAML_UI_CANVAS_DIRECT2D
    if (XAML_FAILED(xaml_object_init<xaml_canvas_d2d_impl>(ptr)))
    {
        return xaml_object_init<xaml_canvas_gdiplus_impl>(ptr);
    }
    return XAML_S_OK;
#else
    return xaml_object_init<xaml_canvas_gdiplus_impl>(ptr);
#endif // XAML_UI_CANVAS_DIRECT2D
}
