#include <shared/menu_bar.hpp>
#include <shared/window.hpp>
#include <xaml/ui/window.h>

using namespace std;

xaml_window_internal::xaml_window_internal() noexcept : xaml_container_internal(), m_is_resizable(true)
{
}

xaml_result xaml_window_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_container_internal::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_is_resizable_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_location_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_title_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_closing));

    int32_t token;
    XAML_RETURN_IF_FAILED((m_location_changed->add(
        [this](xaml_object*, xaml_point) noexcept -> xaml_result {
            if (m_handle && !m_resizing) XAML_RETURN_IF_FAILED(draw({}));
            return XAML_S_OK;
        },
        &token)));
    XAML_RETURN_IF_FAILED((m_is_resizable_changed->add(
        [this](xaml_object*, bool) noexcept -> xaml_result {
            if (m_handle) XAML_RETURN_IF_FAILED(draw_resizable());
            return XAML_S_OK;
        },
        &token)));
    XAML_RETURN_IF_FAILED((m_title_changed->add(
        [this](xaml_object*, xaml_string*) noexcept -> xaml_result {
            if (m_handle) XAML_RETURN_IF_FAILED(draw_title());
            return XAML_S_OK;
        },
        &token)));
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_window_new(xaml_window** ptr) noexcept
{
    return xaml_object_init<xaml_window_impl>(ptr);
}

xaml_result XAML_CALL xaml_window_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_window;
    XAML_RETURN_IF_FAILED(xaml_container_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_window_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(title, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(location, xaml_point);
    XAML_TYPE_INFO_ADD_PROP(x, double);
    XAML_TYPE_INFO_ADD_PROP(y, double);
    XAML_TYPE_INFO_ADD_EVENT(closing);
    XAML_TYPE_INFO_ADD_PROP_EVENT(is_resizable, bool);
    XAML_TYPE_INFO_ADD_PROP(menu_bar, xaml_control);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_window_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_window, "xaml/ui/window.h");
    XAML_RETURN_IF_FAILED(xaml_window_members(__info));
    return ctx->add_type(__info);
}
