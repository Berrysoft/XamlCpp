#include <xaml/meta/module.h>
#include <xaml/ui/menu_bar.h>
#include <xaml/ui/window.h>

struct xaml_module_info_impl : xaml_implement<xaml_module_info_impl, xaml_module_info, xaml_object>
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

    xaml_result XAML_CALL init() noexcept
    {
        XAML_RETURN_IF_FAILED(xaml_vector_new(&m_dependencies));
        {
            xaml_ptr<xaml_string> dep;
            XAML_RETURN_IF_FAILED(xaml_string_new(U("xaml_markup"), &dep));
            XAML_RETURN_IF_FAILED(m_dependencies->append(dep));
        }
        return XAML_S_OK;
    }
};

EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_module_get_info(xaml_module_info** ptr) noexcept
{
    return xaml_object_init<xaml_module_info_impl>(ptr);
}
