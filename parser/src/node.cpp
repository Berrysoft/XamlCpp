#include <xaml/parser/node.h>

struct xaml_node_base_internal
{
    xaml_object* m_outer_this{};
    virtual ~xaml_node_base_internal() {}

    XAML_PROP_PTR_IMPL(type, xaml_type_info)
    XAML_PROP_PTR_IMPL(name, xaml_string)
    XAML_PROP_PTR_IMPL(key, xaml_string)
};

template <typename T, typename Internal, typename... Base>
struct xaml_node_base_implement : xaml_implement<T, Base..., xaml_node_base, xaml_object>
{
    Internal m_internal;

    xaml_node_base_implement() noexcept
    {
        m_internal.m_outer_this = this;
    }

    XAML_PROP_PTR_INTERNAL_IMPL(type, xaml_type_info)
    XAML_PROP_PTR_INTERNAL_IMPL(name, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(key, xaml_string)
};

struct xaml_string_node_internal : xaml_node_base_internal
{
    XAML_PROP_PTR_IMPL(value, xaml_string)
};

struct xaml_string_node_impl : xaml_node_base_implement<xaml_string_node_impl, xaml_string_node_internal, xaml_string_node>
{
    XAML_PROP_PTR_INTERNAL_IMPL(value, xaml_string)
};

xaml_result XAML_CALL xaml_string_node_new(xaml_string_node** ptr) noexcept
{
    return xaml_object_new<xaml_string_node_impl>(ptr);
}

struct xaml_markup_node_internal : xaml_node_base_internal
{
    XAML_PROP_PTR_IMPL(properties, xaml_vector)
};

struct xaml_markup_node_impl : xaml_node_base_implement<xaml_markup_node_impl, xaml_markup_node_internal, xaml_markup_node>
{
    XAML_PROP_PTR_INTERNAL_IMPL(properties, xaml_vector)
};

xaml_result XAML_CALL xaml_markup_node_new(xaml_markup_node** ptr) noexcept
{
    return xaml_object_new<xaml_markup_node_impl>(ptr);
}

struct xaml_node_internal : xaml_node_base_internal
{
    XAML_PROP_PTR_IMPL(resources, xaml_map)
    XAML_PROP_PTR_IMPL(properties, xaml_vector)
    XAML_PROP_PTR_IMPL(collection_properties, xaml_map)
    XAML_PROP_PTR_IMPL(events, xaml_vector)
};

struct xaml_node_impl : xaml_node_base_implement<xaml_node_impl, xaml_node_internal, xaml_node>
{
    XAML_PROP_PTR_INTERNAL_IMPL(resources, xaml_map)
    XAML_PROP_PTR_INTERNAL_IMPL(properties, xaml_vector)
    XAML_PROP_PTR_INTERNAL_IMPL(collection_properties, xaml_map)
    XAML_PROP_PTR_INTERNAL_IMPL(events, xaml_vector)
};

xaml_result XAML_CALL xaml_node_new(xaml_node** ptr) noexcept
{
    return xaml_object_new<xaml_node_impl>(ptr);
}

struct xaml_attribute_event_impl : xaml_implement<xaml_attribute_event_impl, xaml_attribute_event, xaml_object>
{
    XAML_PROP_PTR_IMPL_BASE(info, xaml_event_info)
    XAML_PROP_PTR_IMPL_BASE(value, xaml_string)

    xaml_attribute_event_impl(xaml_ptr<xaml_event_info> const& info, xaml_ptr<xaml_string> const& value) noexcept
        : m_info(info), m_value(value) {}
};

xaml_result XAML_CALL xaml_attribute_event_new(xaml_event_info* info, xaml_string* value, xaml_attribute_event** ptr) noexcept
{
    return xaml_object_new<xaml_attribute_event_impl>(ptr, info, value);
}

struct xaml_attribute_property_impl : xaml_implement<xaml_attribute_property_impl, xaml_attribute_property, xaml_object>
{
    XAML_PROP_PTR_IMPL_BASE(type, xaml_type_info)
    XAML_PROP_PTR_IMPL_BASE(info, xaml_property_info)
    XAML_PROP_PTR_IMPL_BASE(value, xaml_node_base)

    xaml_attribute_property_impl(xaml_ptr<xaml_type_info> const& type, xaml_ptr<xaml_property_info> const& info, xaml_ptr<xaml_node_base> const& value) noexcept
        : m_type(type), m_info(info), m_value(value) {}
};

xaml_result XAML_CALL xaml_attribute_property_new(xaml_type_info* type, xaml_property_info* info, xaml_node_base* value, xaml_attribute_property** ptr) noexcept
{
    return xaml_object_new<xaml_attribute_property_impl>(ptr, type, info, value);
}

struct xaml_attribute_collection_property_impl : xaml_implement<xaml_attribute_collection_property_impl, xaml_attribute_collection_property, xaml_object>
{
    XAML_PROP_PTR_IMPL_BASE(type, xaml_type_info)
    XAML_PROP_PTR_IMPL_BASE(info, xaml_collection_property_info)
    XAML_PROP_PTR_IMPL_BASE(values, xaml_vector)

    xaml_attribute_collection_property_impl(xaml_ptr<xaml_type_info> const& type, xaml_ptr<xaml_collection_property_info> const& info, xaml_ptr<xaml_vector> const& values) noexcept
        : m_type(type), m_info(info), m_values(values) {}
};

xaml_result XAML_CALL xaml_attribute_collection_property_new(xaml_type_info* type, xaml_collection_property_info* info, xaml_vector* values, xaml_attribute_collection_property** ptr) noexcept
{
    return xaml_object_new<xaml_attribute_collection_property_impl>(ptr, type, info, values);
}
