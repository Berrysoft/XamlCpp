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

    xaml_result XAML_CALL invoke(xaml_vector_view<xaml_object>* args) noexcept override
    {
        return m_func(args);
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

xaml_result XAML_CALL xaml_method_info_bind(xaml_method_info* func, xaml_vector_view<xaml_object>* args, xaml_method_info** ptr) noexcept
{
    xaml_ptr<xaml_string> name;
    XAML_RETURN_IF_FAILED(func->get_name(&name));
    xaml_ptr<xaml_vector_view<xaml_guid>> param_types;
    XAML_RETURN_IF_FAILED(func->get_param_types(&param_types));
    int32_t size;
    XAML_RETURN_IF_FAILED(args->get_size(&size));
    int32_t psize;
    XAML_RETURN_IF_FAILED(param_types->get_size(&psize));
    xaml_ptr<xaml_vector<xaml_guid>> npts;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&npts));
    for (int32_t i = size; i < psize; i++)
    {
        xaml_guid t;
        XAML_RETURN_IF_FAILED(param_types->get_at(i, &t));
        XAML_RETURN_IF_FAILED(npts->append(t));
    }
    return xaml_object_new_catch<xaml_method_info_impl>(
        ptr, move(name),
        [func = xaml_ptr<xaml_method_info>(func), args = xaml_ptr<xaml_vector_view<xaml_object>>(args)](xaml_vector_view<xaml_object>* remain_args) noexcept -> xaml_result {
            xaml_ptr<xaml_vector<xaml_object>> real_args;
            XAML_RETURN_IF_FAILED(xaml_vector_new(&real_args));
            XAML_FOREACH_START(xaml_object, a, args);
            {
                XAML_RETURN_IF_FAILED(real_args->append(a));
            }
            XAML_FOREACH_END();
            XAML_FOREACH_START(xaml_object, a, remain_args);
            {
                XAML_RETURN_IF_FAILED(real_args->append(a));
            }
            XAML_FOREACH_END();
            return func->invoke(real_args);
        },
        move(npts));
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
