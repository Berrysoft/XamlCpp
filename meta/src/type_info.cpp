#include <xaml/meta/type_info.h>

using namespace std;

struct xaml_type_info_registration_impl : xaml_implement<xaml_type_info_registration_impl, xaml_type_info_registration, xaml_type_info, xaml_reflection_info, xaml_object>
{
private:
    xaml_guid m_type;
    xaml_ptr<xaml_string> m_name;
    xaml_ptr<xaml_string> m_include_file;

public:
    xaml_type_info_registration_impl(xaml_guid const& type, xaml_ptr<xaml_string>&& name, xaml_ptr<xaml_string>&& include_file) noexcept
        : m_type(type), m_name(move(name)), m_include_file(move(include_file)) {}

    xaml_result XAML_CALL get_type(xaml_guid* ptype) noexcept override
    {
        *ptype = m_type;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_name(xaml_string** ptr) noexcept override
    {
        m_name->add_ref();
        *ptr = m_name.get();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_include_file(xaml_string** ptr) noexcept override
    {
        m_include_file->add_ref();
        *ptr = m_include_file.get();
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
