#include <win/canvas_gdiplus.hpp>
#include <xaml/ui/controls/canvas.h>

#ifdef XAML_UI_CANVAS_DIRECT2D
#include <win/canvas_d2d.hpp>
#endif // XAML_UI_CANVAS_DIRECT2D

using namespace std;

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
