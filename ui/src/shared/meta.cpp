#include <xaml/ui/menu_bar.h>
#include <xaml/ui/window.h>
#include <xaml/version.h>

EXTERN_C XAML_UI_API xaml_result xaml_module_version(xaml_version* pver) noexcept
{
    *pver = xaml_version_current;
    return XAML_S_OK;
}

EXTERN_C XAML_UI_API xaml_result xaml_module_dependencies(char const* const** parr) noexcept
{
    static char const* arr[] = { U("xaml_markup"), nullptr };
    *parr = arr;
    return XAML_S_OK;
}

EXTERN_C XAML_UI_API xaml_result xaml_module_register(xaml_meta_context* ctx) noexcept
{
    xaml_ptr<xaml_string> xml_ns;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("https://github.com/Berrysoft/XamlCpp/"), &xml_ns));
    xaml_ptr<xaml_string> ns;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("xaml"), &ns));
    XAML_RETURN_IF_FAILED(ctx->add_namespace(xml_ns, ns));
    XAML_RETURN_IF_FAILED(xaml_control_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_container_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_multicontainer_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_window_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_menu_bar_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_halignment_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_valignment_register(ctx));
    XAML_META_CONTEXT_ADD_BASIC_TYPE(xaml_size);
    XAML_META_CONTEXT_ADD_BASIC_TYPE(xaml_point);
    XAML_META_CONTEXT_ADD_BASIC_TYPE(xaml_rectangle);
    XAML_META_CONTEXT_ADD_BASIC_TYPE(xaml_margin);
    return XAML_S_OK;
}
