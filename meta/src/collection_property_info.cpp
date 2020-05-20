#include <xaml/meta/collection_property_info.h>

using namespace std;

struct xaml_collection_property_info_impl : xaml_implement<xaml_collection_property_info_impl, xaml_collection_property_info, xaml_object>
{
private:
    xaml_ptr<xaml_string> m_name;
    xaml_guid m_type;
    function<xaml_result(xaml_object*, xaml_object*)> m_adder;
    function<xaml_result(xaml_object*, xaml_object*)> m_remover;

public:
    xaml_collection_property_info_impl(xaml_ptr<xaml_string>&& name, xaml_guid const& type, function<xaml_result(xaml_object*, xaml_object*)>&& adder, function<xaml_result(xaml_object*, xaml_object*)>&& remover) noexcept
        : m_name(move(name)), m_type(type), m_adder(move(adder)), m_remover(move(remover)) {}

    xaml_result XAML_CALL get_name(xaml_string** ptr) noexcept override
    {
        return m_name->query(ptr);
    }

    xaml_result XAML_CALL get_type(xaml_guid* ptype) noexcept override
    {
        *ptype = m_type;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_can_add(bool* pb) noexcept override
    {
        *pb = (bool)m_adder;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_can_remove(bool* pb) noexcept override
    {
        *pb = (bool)m_remover;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL add(xaml_object* target, xaml_object* obj) noexcept override
    {
        if (m_adder)
        {
            return m_adder(target, obj);
        }
        return XAML_E_NOTIMPL;
    }

    xaml_result XAML_CALL remove(xaml_object* target, xaml_object* obj) noexcept override
    {
        if (m_remover)
        {
            return m_remover(target, obj);
        }
        return XAML_E_NOTIMPL;
    }
};

xaml_result XAML_CALL xaml_collection_property_info_new(xaml_string* name, xaml_guid const& type, xaml_result(XAML_CALL* adder)(xaml_object*, xaml_object*), xaml_result(XAML_CALL* remover)(xaml_object*, xaml_object*), xaml_collection_property_info** ptr) noexcept
{
    return xaml_object_new_catch<xaml_collection_property_info_impl>(ptr, name, type, adder, remover);
}

xaml_result XAML_CALL xaml_collection_property_info_new(xaml_string* name, xaml_guid const& type, function<xaml_result(xaml_object*, xaml_object*)>&& adder, function<xaml_result(xaml_object*, xaml_object*)>&& remover, xaml_collection_property_info** ptr) noexcept
{
    return xaml_object_new<xaml_collection_property_info_impl>(ptr, name, type, move(adder), move(remover));
}
