#include <xaml/meta/method_info.h>

using namespace std;

struct xaml_method_info_impl : xaml_implement<xaml_method_info_impl, xaml_method_info>
{
    xaml_ptr<xaml_string> m_name;
    function<xaml_result(xaml_vector_view<xaml_object>*)> m_func;
    xaml_ptr<xaml_vector_view<xaml_guid>> m_param_types;

    xaml_method_info_impl(xaml_ptr<xaml_string>&& name, function<xaml_result(xaml_vector_view<xaml_object>*)>&& func, xaml_ptr<xaml_vector_view<xaml_guid>>&& param_types) noexcept
        : m_name(move(name)), m_func(move(func)), m_param_types(move(param_types)) {}

    xaml_result XAML_CALL get_name(xaml_string** ptr) noexcept override
    {
        return m_name->query(ptr);
    }

    xaml_result XAML_CALL invoke(xaml_vector_view<xaml_object>* args, xaml_object** ptr) noexcept override
    {
        XAML_RETURN_IF_FAILED(m_func(args));
        *ptr = nullptr;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_param_types(xaml_vector_view<xaml_guid>** ptr) noexcept override
    {
        return m_param_types->query(ptr);
    }
};

xaml_result XAML_CALL xaml_method_info_new(xaml_string* name, xaml_result(XAML_CALL* func)(xaml_vector_view<xaml_object>*), xaml_vector_view<xaml_guid>* param_types, xaml_method_info** ptr) noexcept
{
    return xaml_object_new_catch<xaml_method_info_impl>(ptr, name, func, param_types);
}

xaml_result XAML_CALL xaml_method_info_new(xaml_string* name, std::function<xaml_result(xaml_vector_view<xaml_object>*)>&& func, xaml_vector_view<xaml_guid>* param_types, xaml_method_info** ptr) noexcept
{
    return xaml_object_new<xaml_method_info_impl>(ptr, name, move(func), param_types);
}

struct xaml_constructor_info_impl : xaml_implement<xaml_constructor_info_impl, xaml_constructor_info>
{
    xaml_guid m_type;
    function<xaml_result(xaml_object**)> m_func;

    xaml_constructor_info_impl(xaml_guid const& type, function<xaml_result(xaml_object**)>&& func) noexcept
        : m_type(type), m_func(move(func)) {}

    xaml_result XAML_CALL invoke(xaml_object** ptr) noexcept override
    {
        return m_func(ptr);
    }

    xaml_result XAML_CALL get_type(xaml_guid* ptr) noexcept override
    {
        *ptr = m_type;
        return XAML_S_OK;
    }
};

xaml_result XAML_CALL xaml_constructor_info_new(xaml_guid const& type, xaml_result(XAML_CALL* func)(xaml_object**), xaml_constructor_info** ptr) noexcept
{
    return xaml_object_new_catch<xaml_constructor_info_impl>(ptr, type, func);
}

xaml_result XAML_CALL xaml_constructor_info_new(xaml_guid const& type, function<xaml_result(xaml_object**)>&& func, xaml_constructor_info** ptr) noexcept
{
    return xaml_object_new<xaml_constructor_info_impl>(ptr, type, move(func));
}
