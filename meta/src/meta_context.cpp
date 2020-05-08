#include <xaml/meta/meta_context.h>

struct xaml_meta_context_impl : xaml_implement<xaml_meta_context_impl, xaml_meta_context, xaml_object>
{
private:
    xaml_ptr<xaml_vector> m_modules;
    xaml_ptr<xaml_map> m_namespace;
    xaml_ptr<xaml_map> m_type_info_map;
    xaml_ptr<xaml_map> m_name_info_map;

public:
    xaml_result init() noexcept
    {
        XAML_RETURN_IF_FAILED(xaml_vector_new(&m_modules));
        xaml_ptr<xaml_hasher> guid_hasher;
        XAML_RETURN_IF_FAILED(xaml_hasher_new<xaml_guid>(&guid_hasher));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(guid_hasher.get(), &m_type_info_map));
        xaml_ptr<xaml_hasher> string_hasher;
        XAML_RETURN_IF_FAILED(xaml_hasher_string_default(&string_hasher));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(string_hasher.get(), &m_namespace));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(string_hasher.get(), &m_name_info_map));
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_modules(xaml_vector_view** ptr) noexcept override
    {
        return m_modules->query(ptr);
    }

    xaml_result XAML_CALL add_module(xaml_module* mod) noexcept override
    {
        return m_modules->append(mod);
    }

    xaml_result XAML_CALL get_namespace(xaml_string* xml_ns, xaml_string** ptr) noexcept override
    {
        xaml_ptr<xaml_object> item;
        XAML_RETURN_IF_FAILED(m_namespace->lookup(xml_ns, &item));
        return item->query(&ptr);
    }

    xaml_result XAML_CALL add_namespace(xaml_string* xml_ns, xaml_string* ns) noexcept override
    {
        bool replaced;
        return m_namespace->insert(xml_ns, ns, &replaced);
    }

    xaml_result XAML_CALL get_types(xaml_map_view** ptr) noexcept override
    {
        return m_type_info_map->query(ptr);
    }

    xaml_result XAML_CALL get_type(xaml_guid const& type, xaml_reflection_info** ptr) noexcept override
    {
        xaml_ptr<xaml_object> key;
        XAML_RETURN_IF_FAILED(xaml_box_value(type, &key));
        return m_type_info_map->lookup(key.get(), (xaml_object**)ptr);
    }

    xaml_result XAML_CALL get_type_by_name(xaml_string* name, xaml_reflection_info** ptr) noexcept override
    {
        xaml_ptr<xaml_object> item;
        XAML_RETURN_IF_FAILED(m_name_info_map->lookup(name, &item));
        return item->query(ptr);
    }

    xaml_result XAML_CALL get_type_by_namespace_name(xaml_string* ns, xaml_string* name, xaml_reflection_info** ptr) noexcept override
    {
        xaml_ptr<xaml_string> real_name;
        XAML_RETURN_IF_FAILED(get_name_by_namespace_name(ns, name, &real_name));
        return get_type_by_name(real_name.get(), ptr);
    }

    xaml_result XAML_CALL get_name_by_namespace_name(xaml_string* ns, xaml_string* name, xaml_string** ptr) noexcept override
    {
        xaml_ptr<xaml_string> real_ns = ns;
        xaml_ptr<xaml_object> item;
        if (XAML_SUCCEEDED(m_namespace->lookup(ns, &item)))
        {
            real_ns = nullptr;
            XAML_RETURN_IF_FAILED(item->query(&real_ns));
        }
        xaml_std_string_view_t ns_view;
        XAML_RETURN_IF_FAILED(xaml_unbox_value(real_ns.get(), ns_view));
        xaml_std_string_view_t name_view;
        XAML_RETURN_IF_FAILED(xaml_unbox_value(name, name_view));
        xaml_ptr<xaml_string> real_name;
        XAML_RETURN_IF_FAILED(xaml_string_new((xaml_std_string_t)ns_view + U("_") + (xaml_std_string_t)name_view, &real_name));
        return real_name->query(ptr);
    }

    xaml_result XAML_CALL add_type(xaml_reflection_info* info) noexcept override
    {
        xaml_guid type;
        XAML_RETURN_IF_FAILED(info->get_type(&type));
        xaml_ptr<xaml_string> name;
        XAML_RETURN_IF_FAILED(info->get_name(&name));
        xaml_ptr<xaml_object> key;
        XAML_RETURN_IF_FAILED(xaml_box_value(type, &key));
        bool replaced;
        XAML_RETURN_IF_FAILED(m_type_info_map->insert(key.get(), info, &replaced));
        return m_name_info_map->insert(name.get(), info, &replaced);
    }

    xaml_result XAML_CALL bind(xaml_object*, xaml_string*, xaml_object*, xaml_string*, xaml_binding_mode = xaml_binding_one_time) noexcept override
    {
        // TODO
        return XAML_S_OK;
    }
};

xaml_result XAML_CALL xaml_meta_context_new(xaml_meta_context** ptr) noexcept
{
    return xaml_object_init<xaml_meta_context_impl>(ptr);
}
