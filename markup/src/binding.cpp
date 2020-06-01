#include <xaml/markup/binding.h>

using namespace std;

struct xaml_binding_impl : xaml_implement<xaml_binding_impl, xaml_binding, xaml_markup_extension, xaml_object>
{
    XAML_PROP_PTR_IMPL(element, xaml_string)
    XAML_PROP_PTR_IMPL(path, xaml_string)
    XAML_PROP_IMPL(mode, xaml_binding_mode, xaml_binding_mode*, xaml_binding_mode)
    XAML_PROP_PTR_IMPL(converter, xaml_converter)
    XAML_PROP_PTR_IMPL(converter_parameter, xaml_object)
    XAML_PROP_PTR_IMPL(converter_language, xaml_string)

    xaml_result XAML_CALL provide(xaml_meta_context* ctx, xaml_markup_context* mkctx) noexcept override
    {
        xaml_ptr<xaml_object> current_element;
        XAML_RETURN_IF_FAILED(mkctx->get_current_element(&current_element));
        xaml_ptr<xaml_string> current_property;
        XAML_RETURN_IF_FAILED(mkctx->get_current_property(&current_property));
        xaml_ptr<xaml_object> element;
        XAML_RETURN_IF_FAILED(mkctx->find_element(m_element, &element));
        return ctx->bind(current_element, current_property, element, m_path, m_mode, m_converter, m_converter_parameter, m_converter_language);
    }
};

xaml_result XAML_CALL xaml_binding_new(xaml_binding** ptr) noexcept
{
    return xaml_object_new<xaml_binding_impl>(ptr);
}

xaml_result XAML_CALL xaml_binding_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_binding;
    XAML_TYPE_INFO_ADD_CTOR(xaml_binding_new);
    XAML_TYPE_INFO_ADD_PROP(element, xaml_string);
    XAML_TYPE_INFO_ADD_PROP(path, xaml_string);
    XAML_TYPE_INFO_ADD_PROP(mode, xaml_binding_mode);
    XAML_TYPE_INFO_ADD_PROP(converter, xaml_converter);
    XAML_TYPE_INFO_ADD_PROP(converter_parameter, xaml_object);
    XAML_TYPE_INFO_ADD_PROP(converter_language, xaml_string);
    XAML_TYPE_INFO_ADD_DEF_PROP(path);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_binding_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_binding, "xaml/markup/binding.h");
    XAML_RETURN_IF_FAILED(xaml_binding_members(__info));
    return ctx->add_type(__info);
}

xaml_result XAML_CALL xaml_binding_mode_register(xaml_meta_context* ctx) noexcept
{
    XAML_ENUM_INFO_MAP_NEW();
    XAML_ENUM_INFO_ADD2(xaml_binding, one_time);
    XAML_ENUM_INFO_ADD2(xaml_binding, one_way);
    XAML_ENUM_INFO_ADD2(xaml_binding, one_way_to_source);
    XAML_ENUM_INFO_ADD2(xaml_binding, two_way);
    XAML_ENUM_INFO_NEW(xaml_binding_mode, "xaml/meta/meta_context.h");
    return ctx->add_type(__info);
}
