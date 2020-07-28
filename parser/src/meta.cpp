#include <xaml/parser/data_template.h>

struct xaml_module_info_impl : xaml_implement<xaml_module_info_impl, xaml_module_info, xaml_object>
{
    xaml_result XAML_CALL get_version(xaml_version* pver) noexcept override
    {
        *pver = xaml_version_current;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_dependencies(xaml_vector_view** ptr) noexcept override
    {
        *ptr = nullptr;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL register_types(xaml_meta_context* ctx) noexcept override
    {
        XAML_RETURN_IF_FAILED(xaml_data_template_register(ctx));
        return XAML_S_OK;
    }
};

EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_module_get_info(xaml_module_info** ptr) noexcept
{
    return xaml_object_new<xaml_module_info_impl>(ptr);
}
