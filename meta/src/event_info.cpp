#include <xaml/meta/event_info.h>
#include <xaml/ptr.hpp>

#include <function.hpp>

using namespace std;

struct xaml_event_info_impl : xaml_implement<xaml_event_info_impl, xaml_event_info>
{
    using adder_func = __xaml_unique_function_wrapper_t<xaml_result(xaml_object*, xaml_method_info*, int32_t*) noexcept>;
    using remover_func = __xaml_unique_function_wrapper_t<xaml_result(xaml_object*, int32_t) noexcept>;

    xaml_ptr<xaml_string> m_name;
    adder_func m_adder;
    remover_func m_remover;

    xaml_event_info_impl(xaml_ptr<xaml_string>&& name, adder_func&& adder, remover_func&& remover) noexcept
        : m_name(move(name)), m_adder(move(adder)), m_remover(move(remover)) {}

    xaml_result XAML_CALL get_name(xaml_string** ptr) noexcept override
    {
        return m_name->query(ptr);
    }

    xaml_result XAML_CALL add(xaml_object* target, xaml_method_info* handler, int32_t* ptoken) noexcept override
    {
        if (m_adder)
        {
            return m_adder(target, handler, ptoken);
        }
        return XAML_E_NOTIMPL;
    }

    xaml_result XAML_CALL remove(xaml_object* target, int32_t token) noexcept override
    {
        if (m_remover)
        {
            return m_remover(target, token);
        }
        return XAML_E_NOTIMPL;
    }
};

xaml_result XAML_CALL xaml_event_info_new(xaml_string* name, xaml_result(XAML_CALL* adder)(xaml_object*, xaml_method_info*, int32_t*) noexcept, xaml_result(XAML_CALL* remover)(xaml_object*, int32_t) noexcept, xaml_event_info** ptr) noexcept
{
    return xaml_object_new_catch<xaml_event_info_impl>(ptr, name, adder, remover);
}

xaml_result XAML_CALL xaml_event_info_new(xaml_string* name, __xaml_unique_function_wrapper_t<xaml_result(xaml_object*, xaml_method_info*, int32_t*) noexcept>&& adder, __xaml_unique_function_wrapper_t<xaml_result(xaml_object*, int32_t) noexcept>&& remover, xaml_event_info** ptr) noexcept
{
    return xaml_object_new<xaml_event_info_impl>(ptr, name, move(adder), move(remover));
}

#ifdef XAML_FUNCTION2
xaml_result XAML_CALL xaml_event_info_new(xaml_string* name, function<xaml_result(xaml_object*, xaml_method_info*, int32_t*)>&& adder, function<xaml_result(xaml_object*, int32_t)>&& remover, xaml_event_info** ptr) noexcept
try
{
    return xaml_object_new<xaml_event_info_impl>(ptr, name, xaml_function_wrap_unique(move(adder)), xaml_function_wrap_unique(move(remover)));
}
XAML_CATCH_RETURN()
#endif // XAML_FUNCTION2
