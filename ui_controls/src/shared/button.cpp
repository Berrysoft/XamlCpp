#include <shared/button.hpp>
#include <xaml/ui/controls/button.h>

using namespace std;

xaml_result xaml_button_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_control_internal::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_text_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_is_default_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_click));

    int32_t token;
    XAML_RETURN_IF_FAILED((m_text_changed->add_noexcept<xaml_ptr<xaml_button>, xaml_ptr<xaml_string>>(
        [this](xaml_ptr<xaml_button>, xaml_ptr<xaml_string>) -> xaml_result {
            if (m_handle)
            {
                XAML_RETURN_IF_FAILED(draw_text());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));
    XAML_RETURN_IF_FAILED((m_is_default_changed->add_noexcept<xaml_ptr<xaml_button>, bool>(
        [this](xaml_ptr<xaml_button>, bool) -> xaml_result {
            if (m_handle) XAML_RETURN_IF_FAILED(draw_default());
            return XAML_S_OK;
        },
        &token)));
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_button_new(xaml_button** ptr) noexcept
{
    return xaml_object_init<xaml_button_impl>(ptr);
}

xaml_result XAML_CALL xaml_button_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_button;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_button_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(text, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(is_default, bool);
    XAML_TYPE_INFO_ADD_EVENT(click);
    XAML_TYPE_INFO_ADD_DEF_PROP(text);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_button_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_button, "xaml/ui/controls/button.h");
    XAML_RETURN_IF_FAILED(xaml_button_members(__info.get()));
    return ctx->add_type(__info.get());
}
