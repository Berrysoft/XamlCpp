#include <xaml/meta/module.h>
#include <xaml/ui/controls/webview.h>

struct xaml_module_info_impl : xaml_implement<xaml_module_info_impl, xaml_module_info>
{
    xaml_ptr<xaml_vector> m_dependencies;

    xaml_result XAML_CALL get_version(xaml_version* pver) noexcept override
    {
        *pver = xaml_version_current;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_dependencies(xaml_vector_view** ptr) noexcept override
    {
        return m_dependencies->query(ptr);
    }

    xaml_result XAML_CALL register_types(xaml_meta_context* ctx) noexcept override
    {
        XAML_RETURN_IF_FAILED(xaml_webview_register(ctx));
        return XAML_S_OK;
    }

    xaml_result XAML_CALL init() noexcept
    {
        XAML_RETURN_IF_FAILED(xaml_vector_new(&m_dependencies));
        {
            xaml_ptr<xaml_string> dep;
            XAML_RETURN_IF_FAILED(xaml_string_new(U("xaml_ui"), &dep));
            XAML_RETURN_IF_FAILED(m_dependencies->append(dep));
        }
        return XAML_S_OK;
    }
};

EXTERN_C XAML_UI_WEBVIEW_API xaml_result XAML_CALL xaml_module_get_info(xaml_module_info** ptr) noexcept
{
    return xaml_object_init<xaml_module_info_impl>(ptr);
}
