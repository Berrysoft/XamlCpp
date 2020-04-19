#include <xaml/meta/meta_context.h>

struct xaml_meta_context_impl : xaml_implement<xaml_meta_context_impl, xaml_meta_context, xaml_object>
{
private:
    xaml_ptr<xaml_vector> m_modules;
    xaml_ptr<xaml_map> m_type_info_map;
    xaml_ptr<xaml_map> m_name_info_map;

public:
    xaml_meta_context_impl()
    {
        XAML_THROW_IF_FAILED(xaml_vector_new(&m_modules));
        xaml_ptr<xaml_hasher> guid_hasher;
        XAML_THROW_IF_FAILED(xaml_hasher_new<xaml_guid>(&guid_hasher));
        XAML_THROW_IF_FAILED(xaml_map_new_with_hasher(guid_hasher.get(), &m_type_info_map));
        xaml_ptr<xaml_hasher> string_hasher;
        XAML_THROW_IF_FAILED(xaml_hasher_string_default(&string_hasher));
        XAML_THROW_IF_FAILED(xaml_map_new_with_hasher(string_hasher.get(), &m_name_info_map));
    }

    xaml_result XAML_CALL get_modules(xaml_vector_view** ptr) noexcept override
    {
        m_modules.add_ref_to(ptr);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL add_module(xaml_module* mod) noexcept override
    {
        return m_modules->append(mod);
    }

    xaml_result XAML_CALL get_types(xaml_map_view** ptr) noexcept override
    {
        m_type_info_map.add_ref_to(ptr);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_type(xaml_guid const& type, xaml_reflection_info** ptr) noexcept override
    {
        xaml_ptr<xaml_object> key;
        XAML_RETURN_IF_FAILED(box_value(type, &key));
        return m_type_info_map->lookup(key.get(), (xaml_object**)ptr);
    }

    xaml_result XAML_CALL get_type_by_name(xaml_string* name, xaml_reflection_info** ptr) noexcept override
    {
        return m_name_info_map->lookup(name, (xaml_object**)ptr);
    }

    xaml_result XAML_CALL add_type(xaml_reflection_info* info) noexcept override
    {
        xaml_guid type;
        XAML_RETURN_IF_FAILED(info->get_type(&type));
        xaml_ptr<xaml_string> name;
        XAML_RETURN_IF_FAILED(info->get_name(&name));
        xaml_ptr<xaml_object> key;
        XAML_RETURN_IF_FAILED(box_value(type, &key));
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

xaml_result xaml_meta_context_new(xaml_meta_context** ptr) noexcept
{
    return xaml_object_new<xaml_meta_context_impl>(ptr);
}
