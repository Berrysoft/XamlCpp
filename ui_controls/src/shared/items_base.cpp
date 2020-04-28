#include <shared/items_base.hpp>
#include <xaml/ui/controls/items_base.h>

xaml_result XAML_CALL xaml_items_base_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_items_base;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_PROP_EVENT(items);
    XAML_TYPE_INFO_ADD_PROP_EVENT(sel_id);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_items_base_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_items_base, "xaml/ui/controls/items_base.h");
    XAML_RETURN_IF_FAILED(xaml_items_base_members(__info.get()));
    return ctx->add_type(__info.get());
}
