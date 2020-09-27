#include <xaml/markup/binding.h>
#include <xaml/markup/data_template.h>
#include <xaml/markup/dynamic_resource.h>
#include <xaml/markup/element_base.h>
#include <xaml/meta/module.h>

struct xaml_module_info_impl : xaml_implement<xaml_module_info_impl, xaml_module_info>
{
    xaml_result XAML_CALL get_version(xaml_version* pver) noexcept override
    {
        *pver = xaml_version_current;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_dependencies(xaml_vector_view<xaml_string>** ptr) noexcept override
    {
        *ptr = nullptr;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL register_types(xaml_meta_context* ctx) noexcept override
    {
        XAML_RETURN_IF_FAILED(xaml_element_base_register(ctx));
        XAML_RETURN_IF_FAILED(xaml_binding_register(ctx));
        XAML_RETURN_IF_FAILED(xaml_binding_mode_register(ctx));
        XAML_RETURN_IF_FAILED(xaml_dynamic_resource_register(ctx));
        XAML_RETURN_IF_FAILED(xaml_template_base_register(ctx));
        XAML_RETURN_IF_FAILED(xaml_data_template_register(ctx));
        return XAML_S_OK;
    }
};

EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_module_get_info(xaml_module_info** ptr) noexcept
{
    return xaml_object_new<xaml_module_info_impl>(ptr);
}
