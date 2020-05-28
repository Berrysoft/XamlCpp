#include <shared/canvas.hpp>
#include <xaml/ui/controls/canvas.h>

using namespace std;

xaml_result xaml_canvas_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_control_internal::init());
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_redraw));
#ifdef XAML_UI_WINDOWS
    XAML_RETURN_IF_FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d));
    XAML_RETURN_IF_FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, &dwrite));
#endif // XAML_UI_WINDOWS
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_canvas_new(xaml_canvas** ptr) noexcept
{
    return xaml_object_init<xaml_canvas_impl>(ptr);
}

xaml_result XAML_CALL xaml_canvas_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_canvas;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_canvas_new);
    XAML_TYPE_INFO_ADD_EVENT(redraw);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_canvas_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_canvas, "xaml/ui/controls/button.h");
    XAML_RETURN_IF_FAILED(xaml_canvas_members(__info));
    return ctx->add_type(__info);
}
