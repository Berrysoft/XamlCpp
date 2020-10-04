#include <xaml/map.h>
#include <xaml/markup/platform.h>

struct xaml_platform_on_impl : xaml_implement<xaml_platform_on_impl, xaml_platform_on>
{
    XAML_PROP_PTR_IMPL(platform, xaml_string)
    XAML_PROP_PTR_IMPL(value, xaml_object)
};

xaml_result XAML_CALL xaml_platform_on_new(xaml_platform_on** ptr) noexcept
{
    return xaml_object_new<xaml_platform_on_impl>(ptr);
}

xaml_result XAML_CALL xaml_platform_on_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_platform_on;
    XAML_TYPE_INFO_ADD_CTOR(xaml_platform_on_new);
    XAML_TYPE_INFO_ADD_PROP(platform, xaml_string);
    XAML_TYPE_INFO_ADD_PROP(value, xaml_object);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_platform_on_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_platform_on, "xaml/markup/platform_on.h");
    XAML_RETURN_IF_FAILED(xaml_platform_on_members(__info));
    return ctx->add_type(__info);
}

struct xaml_platform_impl : xaml_implement<xaml_platform_impl, xaml_platform>
{
    xaml_ptr<xaml_map<xaml_string, xaml_object>> m_map{};

    xaml_result XAML_CALL add_platform(xaml_platform_on* on) noexcept override
    {
        xaml_ptr<xaml_string> platform;
        XAML_RETURN_IF_FAILED(on->get_platform(&platform));
        xaml_ptr<xaml_object> value;
        XAML_RETURN_IF_FAILED(on->get_value(&value));
        return m_map->insert(platform, value, nullptr);
    }

    xaml_result XAML_CALL remove_platform(xaml_platform_on* on) noexcept override
    {
        xaml_ptr<xaml_string> platform;
        XAML_RETURN_IF_FAILED(on->get_platform(&platform));
        return m_map->remove(platform);
    }

    xaml_result XAML_CALL add_platform_2(xaml_string* platform, xaml_object* value) noexcept override
    {
        return m_map->insert(platform, value, nullptr);
    }

    xaml_result XAML_CALL remove_platform_2(xaml_string* platform) noexcept override
    {
        return m_map->remove(platform);
    }

    xaml_result XAML_CALL provide(xaml_meta_context* ctx, xaml_markup_context* mkctx) noexcept override
    {
        xaml_ptr<xaml_object> current_element;
        XAML_RETURN_IF_FAILED(mkctx->get_current_object(&current_element));
        xaml_ptr<xaml_string> current_property;
        XAML_RETURN_IF_FAILED(mkctx->get_current_property(&current_property));
        xaml_ptr<xaml_type_info> type;
        {
            xaml_guid current_type;
            XAML_RETURN_IF_FAILED(current_element->get_guid(&current_type));
            xaml_ptr<xaml_reflection_info> ref;
            XAML_RETURN_IF_FAILED(ctx->get_type(current_type, &ref));
            XAML_RETURN_IF_FAILED(ref->query(&type));
        }
        xaml_ptr<xaml_property_info> prop;
        XAML_RETURN_IF_FAILED(type->get_property(current_property, &prop));
        xaml_ptr<xaml_string> key;
        XAML_RETURN_IF_FAILED(xaml_string_new(XAML_PLATFORM_CURRENT, &key));
        xaml_ptr<xaml_object> value;
        if (XAML_SUCCEEDED(m_map->lookup(key, &value)))
        {
            XAML_RETURN_IF_FAILED(prop->set(current_element, value));
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL init() noexcept
    {
        return xaml_map_new(&m_map);
    }
};

xaml_result XAML_CALL xaml_platform_new(xaml_platform** ptr) noexcept
{
    return xaml_object_init<xaml_platform_impl>(ptr);
}

xaml_result XAML_CALL xaml_platform_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_platform;
    XAML_TYPE_INFO_ADD_CTOR(xaml_platform_new);
    XAML_TYPE_INFO_ADD_CPROP(platform, xaml_platform);
    XAML_TYPE_INFO_ADD_DEF_PROP(platform);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_platform_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_platform, "xaml/markup/platform.h");
    XAML_RETURN_IF_FAILED(xaml_platform_members(__info));
    return ctx->add_type(__info);
}
