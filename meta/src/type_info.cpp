#include <reflection_info.hpp>
#include <xaml/meta/type_info.h>

using namespace std;

struct xaml_basic_type_info_impl : xaml_reflection_info_implement<xaml_basic_type_info_impl, xaml_basic_type_info>
{
    using xaml_reflection_info_implement::xaml_reflection_info_implement;
};

xaml_result XAML_CALL xaml_basic_type_info_new(xaml_guid const& type, xaml_string* name, xaml_string* include_file, xaml_basic_type_info** ptr) noexcept
{
    return xaml_object_new<xaml_basic_type_info_impl>(ptr, type, name, include_file);
}

struct xaml_type_info_registration_impl : xaml_reflection_info_implement<xaml_type_info_registration_impl, xaml_type_info_registration, xaml_type_info>
{
    xaml_ptr<xaml_map> m_attr_map;
    xaml_ptr<xaml_delegate> m_ctor{ nullptr };
    xaml_ptr<xaml_map> m_method_map;
    xaml_ptr<xaml_map> m_prop_map;
    xaml_ptr<xaml_map> m_cprop_map;
    xaml_ptr<xaml_map> m_event_map;

    using xaml_reflection_info_implement::xaml_reflection_info_implement;

    xaml_result init() noexcept
    {
        xaml_ptr<xaml_hasher> guid_hasher;
        XAML_RETURN_IF_FAILED(xaml_hasher_new<xaml_guid>(&guid_hasher));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(guid_hasher.get(), &m_attr_map));
        xaml_ptr<xaml_hasher> string_hasher;
        XAML_RETURN_IF_FAILED(xaml_hasher_string_default(&string_hasher));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(string_hasher.get(), &m_method_map));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(string_hasher.get(), &m_prop_map));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(string_hasher.get(), &m_cprop_map));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(string_hasher.get(), &m_event_map));
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_attributes(xaml_map_view** ptr) noexcept override
    {
        return m_attr_map->query(ptr);
    }

    xaml_result XAML_CALL get_attribute(xaml_guid const& type, void** ptr) noexcept override
    {
        xaml_ptr<xaml_object> key;
        XAML_RETURN_IF_FAILED(xaml_box_value(type, &key));
        xaml_ptr<xaml_object> item;
        XAML_RETURN_IF_FAILED(m_attr_map->lookup(key.get(), &item));
        return item->query(type, ptr);
    }

    xaml_result XAML_CALL add_attribute(xaml_guid const& type, xaml_object* attr) noexcept override
    {
        xaml_ptr<xaml_object> key;
        XAML_RETURN_IF_FAILED(xaml_box_value(type, &key));
        bool replaced;
        return m_attr_map->insert(key.get(), attr, &replaced);
    }

    xaml_result XAML_CALL get_constructor(xaml_delegate** ptr) noexcept override
    {
        if (m_ctor)
        {
            return m_ctor->query(ptr);
        }
        else
        {
            *ptr = nullptr;
            return XAML_S_OK;
        }
    }

    xaml_result XAML_CALL set_constructor(xaml_delegate* d) noexcept override
    {
        m_ctor = d;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_methods(xaml_map_view** ptr) noexcept override
    {
        return m_method_map->query(ptr);
    }

    xaml_result XAML_CALL get_method(xaml_string* name, xaml_method_info** ptr) noexcept override
    {
        xaml_ptr<xaml_object> value;
        XAML_RETURN_IF_FAILED(m_method_map->lookup(name, &value));
        return value->query(ptr);
    }

    xaml_result XAML_CALL add_method(xaml_method_info* method) noexcept override
    {
        xaml_ptr<xaml_string> name;
        XAML_RETURN_IF_FAILED(method->get_name(&name));
        bool replaced;
        return m_method_map->insert(name.get(), method, &replaced);
    }

    xaml_result XAML_CALL get_properties(xaml_map_view** ptr) noexcept override
    {
        return m_prop_map->query(ptr);
    }

    xaml_result XAML_CALL get_property(xaml_string* name, xaml_property_info** ptr) noexcept override
    {
        xaml_ptr<xaml_object> value;
        XAML_RETURN_IF_FAILED(m_prop_map->lookup(name, &value));
        return value->query(ptr);
    }

    xaml_result XAML_CALL add_property(xaml_property_info* prop) noexcept override
    {
        xaml_ptr<xaml_string> name;
        XAML_RETURN_IF_FAILED(prop->get_name(&name));
        bool replaced;
        return m_prop_map->insert(name.get(), prop, &replaced);
    }

    xaml_result XAML_CALL get_collection_properties(xaml_map_view** ptr) noexcept override
    {
        return m_cprop_map->query(ptr);
    }

    xaml_result XAML_CALL get_collection_property(xaml_string* name, xaml_collection_property_info** ptr) noexcept override
    {
        xaml_ptr<xaml_object> value;
        XAML_RETURN_IF_FAILED(m_cprop_map->lookup(name, &value));
        return value->query(ptr);
    }

    xaml_result XAML_CALL add_collection_property(xaml_collection_property_info* prop) noexcept override
    {
        xaml_ptr<xaml_string> name;
        XAML_RETURN_IF_FAILED(prop->get_name(&name));
        bool replaced;
        return m_cprop_map->insert(name.get(), prop, &replaced);
    }

    xaml_result XAML_CALL get_events(xaml_map_view** ptr) noexcept override
    {
        return m_event_map->query(ptr);
    }

    xaml_result XAML_CALL get_event(xaml_string* name, xaml_event_info** ptr) noexcept override
    {
        xaml_ptr<xaml_object> value;
        XAML_RETURN_IF_FAILED(m_event_map->lookup(name, &value));
        return value->query(ptr);
    }

    xaml_result XAML_CALL add_event(xaml_event_info* ev) noexcept override
    {
        xaml_ptr<xaml_string> name;
        XAML_RETURN_IF_FAILED(ev->get_name(&name));
        bool replaced;
        return m_event_map->insert(name.get(), ev, &replaced);
    }
};

xaml_result XAML_CALL xaml_type_info_registration_new(xaml_guid const& type, xaml_string* name, xaml_string* include_file, xaml_type_info_registration** ptr) noexcept
{
    return xaml_object_new_and_init<xaml_type_info_registration_impl>(ptr, type, name, include_file);
}
