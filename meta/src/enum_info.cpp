#include <reflection_info.hpp>
#include <xaml/meta/enum_info.h>

using namespace std;

struct xaml_enum_info_impl : xaml_reflection_info_implement<xaml_enum_info_impl, xaml_enum_info>
{
    xaml_ptr<xaml_map_view> m_map;

    xaml_enum_info_impl(xaml_guid const& type, xaml_ptr<xaml_string>&& name, xaml_ptr<xaml_string>&& include_file, xaml_ptr<xaml_map_view>&& map) noexcept
        : xaml_reflection_info_implement(type, move(name), move(include_file)), m_map(move(map)) {}

    xaml_result XAML_CALL get_values(xaml_map_view** ptr) noexcept override
    {
        return m_map->query(ptr);
    }

    xaml_result XAML_CALL get_value(xaml_string* str, xaml_box** ptr) noexcept override
    {
        xaml_ptr<xaml_object> obj;
        XAML_RETURN_IF_FAILED(m_map->lookup(str, &obj));
        return obj->query(ptr);
    }
};

xaml_result XAML_CALL xaml_enum_info_new(xaml_guid const& type, xaml_string* name, xaml_string* include_file, xaml_map_view* map, xaml_enum_info** ptr) noexcept
{
    return xaml_object_new<xaml_enum_info_impl>(ptr, type, name, include_file, map);
}
