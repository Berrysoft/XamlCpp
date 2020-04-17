#include <xaml/callback.h>
#include <xaml/object.h>

using namespace std;

struct xaml_callback_impl : xaml_implement<xaml_callback_impl, xaml_callback, xaml_object>
{
private:
    xaml_result (*m_func)(xaml_object*, xaml_object*);

public:
    constexpr xaml_callback_impl(xaml_result (*func)(xaml_object*, xaml_object*)) noexcept : m_func{ func } {}

    xaml_result XAML_CALL invoke(xaml_object* sender, xaml_object* args) const noexcept override
    {
        return m_func(sender, args);
    }
};

xaml_result xaml_callback_new(xaml_result (*func)(xaml_object*, xaml_object*), xaml_callback** ptr) noexcept
{
    if (!func) return XAML_E_INVALIDARG;
    return xaml_object_new<xaml_callback_impl>(ptr, func);
}
