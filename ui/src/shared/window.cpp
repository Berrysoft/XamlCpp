#include <shared/menu_bar.hpp>
#include <shared/window.hpp>
#include <xaml/ui/window.h>

using namespace std;

xaml_window_impl::xaml_window_impl() noexcept : xaml_container_implement()
{
}

xaml_result xaml_window_impl::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_container_implement::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_is_resizable_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_location_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_title_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_closing));

    size_t token;
    XAML_RETURN_IF_FAILED((m_size_changed->add<void, xaml_ptr<xaml_window>, xaml_ptr<xaml_string>>(
        [this](xaml_ptr<xaml_window>, xaml_ptr<xaml_string>) {
            if (m_handle) draw_title();
        },
        &token)));
    XAML_RETURN_IF_FAILED((m_location_changed->add<void, xaml_ptr<xaml_window>, xaml_point>(
        [this](xaml_ptr<xaml_window>, xaml_point) {
            if (m_handle && !m_resizing) draw({});
        },
        &token)));
    XAML_RETURN_IF_FAILED((m_is_resizable_changed->add<void, xaml_ptr<xaml_window>, bool>(
        [this](xaml_ptr<xaml_window>, bool) {
            if (m_handle) draw_resizable();
        },
        &token)));
}

xaml_result xaml_window_new(xaml_window** ptr) noexcept
{
    return xaml_object_init<xaml_window_impl>(ptr);
}

xaml_result xaml_window_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_window;
    XAML_RETURN_IF_FAILED(xaml_container_members(__info));
    XAML_TYPE_INFO_ADD_PROP_EVENT(title);
    XAML_TYPE_INFO_ADD_PROP_EVENT(location);
    XAML_TYPE_INFO_ADD_PROP(x);
    XAML_TYPE_INFO_ADD_PROP(y);
    XAML_TYPE_INFO_ADD_EVENT(closing);
    XAML_TYPE_INFO_ADD_PROP_EVENT(is_resizable);
    //XAML_TYPE_INFO_ADD_PROP(menu_bar);
    return XAML_S_OK;
}

xaml_result xaml_window_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_window, "xaml/ui/window.h");
    XAML_RETURN_IF_FAILED(xaml_window_members(__info.get()));
    return ctx->add_type(__info.get());
}
