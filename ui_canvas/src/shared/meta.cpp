#include <xaml/ui/controls/canvas.h>
#include <xaml/version.h>

EXTERN_C XAML_UI_CANVAS_API xaml_result xaml_module_version(xaml_version* pver) noexcept
{
    *pver = xaml_version_current;
    return XAML_S_OK;
}

EXTERN_C XAML_UI_CANVAS_API xaml_result xaml_module_dependencies(char const* const** parr) noexcept
{
    static char const* arr[] = { U("xaml_ui"), nullptr };
    *parr = arr;
    return XAML_S_OK;
}

EXTERN_C XAML_UI_CANVAS_API xaml_result xaml_module_register(xaml_meta_context* ctx) noexcept
{
    XAML_RETURN_IF_FAILED(xaml_canvas_register(ctx));
    return XAML_S_OK;
}
