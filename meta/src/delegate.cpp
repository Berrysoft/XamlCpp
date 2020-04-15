#include <xaml/delegate.h>
#include <xaml/object.hpp>

struct xaml_delegate_impl : xaml_implement<xaml_delegate_impl, xaml_delegate, xaml_delegate, xaml_object>
{
private:
    xaml_result (*m_func)(xaml_vector_view*, xaml_object**);

public:
    constexpr xaml_delegate_impl(xaml_result (*func)(xaml_vector_view*, xaml_object**)) noexcept : m_func{ func } {}

    xaml_result XAML_CALL invoke(xaml_vector_view* args, xaml_object** ptr) const noexcept override
    {
        return m_func(args, ptr);
    }
};

xaml_result xaml_delegate_new(xaml_result (*func)(xaml_vector_view*, xaml_object**), xaml_delegate** ptr) noexcept
{
    xaml_delegate* res = new (nothrow) xaml_delegate_impl(func);
    if (!res) return XAML_E_OUTOFMEMORY;
    *ptr = res;
    return XAML_S_OK;
}
