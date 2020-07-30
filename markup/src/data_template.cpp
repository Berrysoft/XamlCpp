#include <xaml/markup/data_template.h>

xaml_result XAML_CALL xaml_data_template_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_data_template;
    XAML_RETURN_IF_FAILED(xaml_template_base_members(__info));
    XAML_TYPE_INFO_ADD_PROP(converter, xaml_converter);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_data_template_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_data_template, "xaml/parser/data_template.h");
    XAML_RETURN_IF_FAILED(xaml_data_template_members(__info));
    return ctx->add_type(__info);
}
