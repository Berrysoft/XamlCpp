#include <xaml/markup/template_base.h>

xaml_result XAML_CALL xaml_template_base_members(xaml_type_info_registration*) noexcept
{
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_template_base_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_template_base, "xaml/markup/template_base.h");
    XAML_RETURN_IF_FAILED(xaml_template_base_members(__info));
    return ctx->add_type(__info);
}
