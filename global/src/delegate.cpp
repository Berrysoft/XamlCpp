#include <xaml/delegate.h>
#include <xaml/object.hpp>

using namespace std;

struct xaml_delegate_impl : xaml_implement<xaml_delegate_impl, xaml_delegate, xaml_object>
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
    if (!func) return XAML_E_INVALIDARG;
    return xaml_object_new<xaml_delegate_impl>(ptr, func);
}
