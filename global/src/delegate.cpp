#include <xaml/delegate.h>
#include <xaml/object.h>

using namespace std;

struct xaml_delegate_impl : xaml_implement<xaml_delegate_impl, xaml_delegate, xaml_object>
{
private:
    function<xaml_result(xaml_vector_view*, xaml_object**)> m_func;

public:
    xaml_delegate_impl(function<xaml_result(xaml_vector_view*, xaml_object**)>&& func) noexcept : m_func(move(func)) {}

    xaml_result XAML_CALL invoke(xaml_vector_view* args, xaml_object** ptr) noexcept override
    {
        return m_func(args, ptr);
    }
};

xaml_result XAML_CALL xaml_delegate_new(xaml_result(XAML_CALL* func)(xaml_vector_view*, xaml_object**) XAML_NOEXCEPT, xaml_delegate** ptr) noexcept
{
    if (!func) return XAML_E_INVALIDARG;
    return xaml_object_new_catch<xaml_delegate_impl>(ptr, func);
}

xaml_result XAML_CALL xaml_delegate_new(function<xaml_result(xaml_vector_view*, xaml_object**)>&& func, xaml_delegate** ptr) noexcept
{
    if (!func) return XAML_E_INVALIDARG;
    return xaml_object_new<xaml_delegate_impl>(ptr, move(func));
}

xaml_result XAML_CALL xaml_delegate_bind(xaml_delegate* func, xaml_vector_view* args, xaml_delegate** ptr) noexcept
{
    return xaml_object_new_catch<xaml_delegate_impl>(
        ptr,
        [func = xaml_ptr<xaml_delegate>(func), args = xaml_ptr<xaml_vector_view>(args)](xaml_vector_view* remain_args, xaml_object** ptr) noexcept -> xaml_result {
            xaml_ptr<xaml_vector> real_args;
            XAML_RETURN_IF_FAILED(xaml_vector_new(&real_args));
            XAML_FOREACH_START(a, args);
            {
                XAML_RETURN_IF_FAILED(real_args->append(a));
            }
            XAML_FOREACH_END();
            XAML_FOREACH_START(a, remain_args);
            {
                XAML_RETURN_IF_FAILED(real_args->append(a));
            }
            XAML_FOREACH_END();
            return func->invoke(real_args, ptr);
        });
}
