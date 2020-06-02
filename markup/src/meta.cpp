#include <xaml/markup/binding.h>
#include <xaml/markup/dynamic_resource.h>
#include <xaml/markup/element_base.h>
#include <xaml/version.h>

EXTERN_C XAML_MARKUP_API xaml_result xaml_module_version(xaml_version* pver) noexcept
{
    *pver = xaml_version_current;
    return XAML_S_OK;
}

EXTERN_C XAML_MARKUP_API xaml_result xaml_module_register(xaml_meta_context* ctx) noexcept
{
    XAML_RETURN_IF_FAILED(xaml_element_base_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_binding_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_binding_mode_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_dynamic_resource_register(ctx));
    return XAML_S_OK;
}
