#include <xaml/markup/data_template.h>

xaml_result XAML_CALL xaml_data_template_members(xaml_type_info_registration*) noexcept
{
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_data_template_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_data_template, "xaml/markup/data_template.h");
    XAML_RETURN_IF_FAILED(xaml_data_template_members(__info));
    return ctx->add_type(__info);
}
