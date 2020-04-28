#include <shared/entry.hpp>
#include <xaml/ui/controls/entry.h>

using namespace std;

xaml_result XAML_CALL xaml_entry_new(xaml_entry** ptr) noexcept
{
    return xaml_object_init<xaml_entry_impl>(ptr);
}

xaml_result XAML_CALL xaml_entry_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_entry;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_entry_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(text);
    XAML_TYPE_INFO_ADD_PROP(text_halignment);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_entry_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_entry, "xaml/ui/controls/entry.h");
    XAML_RETURN_IF_FAILED(xaml_entry_members(__info.get()));
    return ctx->add_type(__info.get());
}
