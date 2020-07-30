#include <xaml/markup/data_template.h>

struct xaml_data_template_impl : xaml_implement<xaml_data_template_impl, xaml_data_template, xaml_template_base, xaml_object>
{
    XAML_PROP_IMPL(data_type, xaml_guid, xaml_guid*, xaml_guid const&)
    XAML_PROP_PTR_IMPL(converter, xaml_converter)
    XAML_PROP_PTR_IMPL(converter_parameter, xaml_object)
    XAML_PROP_PTR_IMPL(converter_language, xaml_string)

    xaml_result XAML_CALL create(xaml_object* value, xaml_object** ptr) noexcept override
    {
        if (m_converter)
        {
            return m_converter->convert(value, m_data_type, m_converter_parameter, m_converter_language, ptr);
        }
        else if (value)
        {
            return value->query(ptr);
        }
        else
        {
            *ptr = nullptr;
            return XAML_S_OK;
        }
    }
};

xaml_result XAML_CALL xaml_data_template_new(xaml_data_template** ptr) noexcept
{
    return xaml_object_new<xaml_data_template_impl>(ptr);
}

xaml_result XAML_CALL xaml_data_template_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_data_template;
    XAML_RETURN_IF_FAILED(xaml_template_base_members(__info));
    XAML_TYPE_INFO_ADD_PROP(converter, xaml_converter);
    XAML_TYPE_INFO_ADD_PROP(converter_parameter, xaml_object);
    XAML_TYPE_INFO_ADD_PROP(converter_language, xaml_string);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_data_template_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_data_template, "xaml/parser/data_template.h");
    XAML_TYPE_INFO_ADD_CTOR(xaml_data_template_new);
    XAML_RETURN_IF_FAILED(xaml_data_template_members(__info));
    return ctx->add_type(__info);
}
