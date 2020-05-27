#include <shared/password_entry.hpp>
#include <xaml/ui/controls/password_entry.h>

using namespace std;

xaml_result xaml_password_entry_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_entry_internal::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_password_char_changed));

    int32_t token;
    XAML_RETURN_IF_FAILED((m_password_char_changed->add_noexcept<xaml_ptr<xaml_password_entry>, char>(
        [this](xaml_ptr<xaml_password_entry>, char) noexcept -> xaml_result {
            if (m_handle)
            {
                XAML_RETURN_IF_FAILED(draw_password_char());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_password_entry_new(xaml_password_entry** ptr) noexcept
{
    return xaml_object_init<xaml_password_entry_impl>(ptr);
}

xaml_result XAML_CALL xaml_password_entry_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_password_entry;
    XAML_RETURN_IF_FAILED(xaml_entry_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_password_entry_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(password_char, char);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_password_entry_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_password_entry, "xaml/ui/controls/password_entry.h");
    XAML_RETURN_IF_FAILED(xaml_password_entry_members(__info));
    return ctx->add_type(__info);
}
