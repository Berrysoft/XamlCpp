#include <xaml/meta/enum_info.h>

using namespace std;

struct xaml_enum_info_impl : xaml_implement<xaml_enum_info_impl, xaml_enum_info, xaml_reflection_info, xaml_object>
{
private:
    xaml_guid m_type;
    xaml_ptr<xaml_string> m_name;
    xaml_ptr<xaml_string> m_include_file;
    xaml_ptr<xaml_map_view> m_map;

public:
    xaml_enum_info_impl(xaml_guid const& type, xaml_ptr<xaml_string>&& name, xaml_ptr<xaml_string>&& include_file, xaml_ptr<xaml_map_view>&& map) noexcept
        : m_type(type), m_name(move(name)), m_include_file(move(include_file)), m_map(move(map)) {}

    xaml_result XAML_CALL get_type(xaml_guid* ptype) noexcept override
    {
        *ptype = m_type;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_name(xaml_string** ptr) noexcept override
    {
        m_name.add_ref_to(ptr);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_include_file(xaml_string** ptr) noexcept override
    {
        m_include_file.add_ref_to(ptr);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_values(xaml_map_view** ptr) noexcept override
    {
        m_map.add_ref_to(ptr);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_value(xaml_string* str, xaml_box** ptr) noexcept override
    {
        xaml_ptr<xaml_object> obj;
        XAML_RETURN_IF_FAILED(m_map->lookup(str, &obj));
        return obj->query(ptr);
    }
};

xaml_result xaml_enum_info_new(xaml_guid const& type, xaml_string* name, xaml_string* include_file, xaml_map_view* map, xaml_enum_info** ptr) noexcept
{
    return xaml_object_new<xaml_enum_info_impl>(ptr, type, name, include_file, map);
}
