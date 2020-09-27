#include <xaml/markup/element_base.h>

xaml_result XAML_CALL xaml_element_base_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_element_base;
    XAML_TYPE_INFO_ADD_PROP_RD(resources, XAML_MAP_VIEW_T_T_NAME(xaml_string, xaml_object));
    XAML_TYPE_INFO_ADD_EVENT(parent_changed);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_element_base_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_element_base, "xaml/markup/element_base.h");
    XAML_RETURN_IF_FAILED(xaml_element_base_members(__info));
    return ctx->add_type(__info);
}
