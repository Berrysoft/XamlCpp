#include <xaml/meta/type_info.h>

using namespace std;

struct xaml_type_info_registration_impl : xaml_implement<xaml_type_info_registration_impl, xaml_type_info_registration, xaml_type_info, xaml_reflection_info, xaml_object>
{
private:
    xaml_guid m_type;
    xaml_ptr<xaml_string> m_name;
    xaml_ptr<xaml_string> m_include_file;
    xaml_ptr<xaml_map> m_attr_map;
    xaml_ptr<xaml_delegate> m_ctor;
    xaml_ptr<xaml_map> m_method_map;
    xaml_ptr<xaml_map> m_prop_map;
    xaml_ptr<xaml_map> m_cprop_map;
    xaml_ptr<xaml_map> m_event_map;

public:
    xaml_type_info_registration_impl(xaml_guid const& type, xaml_ptr<xaml_string>&& name, xaml_ptr<xaml_string>&& include_file)
        : m_type(type), m_name(move(name)), m_include_file(move(include_file)), m_ctor(nullptr)
    {
        xaml_ptr<xaml_hasher> guid_hasher;
        XAML_THROW_IF_FAILED(xaml_hasher_new<xaml_guid>(&guid_hasher));
        XAML_THROW_IF_FAILED(xaml_map_new_with_hasher(guid_hasher.get(), &m_attr_map));
        xaml_ptr<xaml_hasher> string_hasher;
        XAML_THROW_IF_FAILED(xaml_hasher_string_default(&string_hasher));
        XAML_THROW_IF_FAILED(xaml_map_new_with_hasher(string_hasher.get(), &m_method_map));
        XAML_THROW_IF_FAILED(xaml_map_new_with_hasher(string_hasher.get(), &m_prop_map));
        XAML_THROW_IF_FAILED(xaml_map_new_with_hasher(string_hasher.get(), &m_cprop_map));
        XAML_THROW_IF_FAILED(xaml_map_new_with_hasher(string_hasher.get(), &m_event_map));
    }

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

    xaml_result XAML_CALL get_attributes(xaml_map_view**) noexcept override;
    xaml_result XAML_CALL get_attribute(xaml_guid const&, xaml_object**) noexcept override;
    xaml_result XAML_CALL add_attribute(xaml_guid const&, xaml_object*) noexcept override;
    xaml_result XAML_CALL get_constructor(xaml_delegate**) noexcept override;
    xaml_result XAML_CALL set_constructor(xaml_delegate*) noexcept override;
    xaml_result XAML_CALL get_methods(xaml_map_view**) noexcept override;
    xaml_result XAML_CALL get_method(xaml_string*, xaml_method_info**) noexcept override;
    xaml_result XAML_CALL add_method(xaml_method_info*) noexcept override;
    xaml_result XAML_CALL get_properties(xaml_map_view**) noexcept override;
    xaml_result XAML_CALL get_property(xaml_string*, xaml_property_info**) noexcept override;
    xaml_result XAML_CALL add_property(xaml_property_info*) noexcept override;
    xaml_result XAML_CALL get_collection_properties(xaml_map_view**) noexcept override;
    xaml_result XAML_CALL get_collection_property(xaml_string*, xaml_collection_property_info**) noexcept override;
    xaml_result XAML_CALL add_collection_property(xaml_collection_property_info*) noexcept override;
    xaml_result XAML_CALL get_events(xaml_map_view**) noexcept override;
    xaml_result XAML_CALL get_event(xaml_string*, xaml_event_info**) noexcept override;
    xaml_result XAML_CALL add_event(xaml_event_info*) noexcept override;
};

xaml_result xaml_type_info_registration_new(xaml_guid const& type, xaml_string* name, xaml_string* include_file, xaml_type_info_registration** ptr) noexcept
{
    return xaml_object_new<xaml_type_info_registration_impl>(ptr, type, name, include_file);
}
