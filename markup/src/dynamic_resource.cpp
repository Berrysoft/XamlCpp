#include <xaml/markup/dynamic_resource.h>
#include <xaml/markup/element_base.h>

static xaml_result find_resource(xaml_ptr<xaml_element_base> element, xaml_ptr<xaml_string> const& key, xaml_object** ptr) noexcept
{
    while (element)
    {
        if (XAML_SUCCEEDED(element->get_resource(key, ptr)))
        {
            return XAML_S_OK;
        }
        xaml_element_base* next_element;
        XAML_RETURN_IF_FAILED(element->get_parent(&next_element));
        element = next_element;
    }
    return XAML_E_KEYNOTFOUND;
}

struct xaml_dynamic_resource_impl : xaml_implement<xaml_dynamic_resource_impl, xaml_dynamic_resource>
{
    XAML_PROP_PTR_IMPL(key, xaml_string)

    xaml_result XAML_CALL provide(xaml_meta_context* ctx, xaml_markup_context* mkctx) noexcept override
    {
        xaml_ptr<xaml_object> current_element;
        XAML_RETURN_IF_FAILED(mkctx->get_current_element(&current_element));
        xaml_ptr<xaml_element_base> element;
        XAML_RETURN_IF_FAILED(current_element->query(&element));
        xaml_ptr<xaml_object> res_obj;
        XAML_RETURN_IF_FAILED(find_resource(element, m_key, &res_obj));
        xaml_ptr<xaml_object> current_object;
        XAML_RETURN_IF_FAILED(mkctx->get_current_object(&current_object));
        xaml_ptr<xaml_string> current_property;
        XAML_RETURN_IF_FAILED(mkctx->get_current_property(&current_property));
        xaml_guid type;
        XAML_RETURN_IF_FAILED(current_object->get_guid(&type));
        xaml_ptr<xaml_reflection_info> ref;
        XAML_RETURN_IF_FAILED(ctx->get_type(type, &ref));
        xaml_ptr<xaml_type_info> info;
        XAML_RETURN_IF_FAILED(ref->query(&info));
        xaml_ptr<xaml_property_info> prop_info;
        XAML_RETURN_IF_FAILED(info->get_property(current_property, &prop_info));
        return prop_info->set(current_object, res_obj);
    }
};

xaml_result XAML_CALL xaml_dynamic_resource_new(xaml_dynamic_resource** ptr) noexcept
{
    return xaml_object_new<xaml_dynamic_resource_impl>(ptr);
}

xaml_result XAML_CALL xaml_dynamic_resource_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_dynamic_resource;
    XAML_TYPE_INFO_ADD_CTOR(xaml_dynamic_resource_new);
    XAML_TYPE_INFO_ADD_PROP(key, xaml_string);
    XAML_TYPE_INFO_ADD_DEF_PROP(key);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_dynamic_resource_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_dynamic_resource, "xaml/markup/dynamic_resource.h");
    XAML_RETURN_IF_FAILED(xaml_dynamic_resource_members(__info));
    return ctx->add_type(__info);
}
