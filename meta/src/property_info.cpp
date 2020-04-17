#include <functional>
#include <xaml/meta/property_info.h>
#include <xaml/object.h>
#include <xaml/xaml_ptr.hpp>

using namespace std;

struct xaml_property_info_impl : xaml_implement<xaml_property_info_impl, xaml_property_info, xaml_object>
{
private:
    xaml_ptr<xaml_string> m_name;
    xaml_guid m_type;
    function<xaml_result XAML_CALL(xaml_object*, xaml_object**)> m_getter;
    function<xaml_result XAML_CALL(xaml_object*, xaml_object*)> m_setter;

public:
    xaml_property_info_impl(xaml_ptr<xaml_string>&& name, xaml_guid const& type, function<xaml_result XAML_CALL(xaml_object*, xaml_object**)>&& getter, function<xaml_result XAML_CALL(xaml_object*, xaml_object*)>&& setter)
        : m_name(move(name)), m_type(type), m_getter(move(getter)), m_setter(move(setter)) {}

    xaml_result XAML_CALL get_name(xaml_string** ptr) noexcept override
    {
        m_name->add_ref();
        *ptr = m_name.get();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_type(xaml_guid* ptype) noexcept override
    {
        *ptype = m_type;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_can_read(bool* pb) noexcept override
    {
        *pb = (bool)m_getter;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_can_write(bool* pb) noexcept override
    {
        *pb = (bool)m_setter;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get(xaml_object* target, xaml_object** ptr) noexcept override
    {
        if (m_getter)
        {
            return m_getter(target, ptr);
        }
        return XAML_E_NOTIMPL;
    }

    xaml_result XAML_CALL set(xaml_object* target, xaml_object* obj) noexcept override
    {
        if (m_setter)
        {
            return m_setter(target, obj);
        }
        return XAML_E_NOTIMPL;
    }
};

xaml_result xaml_property_info_new(xaml_string* name, xaml_guid const& type, xaml_result(XAML_CALL* getter)(xaml_object*, xaml_object**), xaml_result(XAML_CALL* setter)(xaml_object*, xaml_object*), xaml_property_info** ptr) noexcept
{
    return xaml_object_new<xaml_property_info_impl>(ptr, name, type, getter, setter);
}

xaml_result xaml_property_info_new(xaml_string* name, xaml_guid const& type, std::function<xaml_result XAML_CALL(xaml_object*, xaml_object**)>&& getter, std::function<xaml_result XAML_CALL(xaml_object*, xaml_object*)>&& setter, xaml_property_info** ptr) noexcept
{
    return xaml_object_new<xaml_property_info_impl>(ptr, name, type, move(getter), move(setter));
}
