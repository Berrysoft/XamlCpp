#include <atomic>
#include <xaml/object.hpp>

using namespace std;

struct xaml_object_impl : xaml_implement<xaml_object_impl, xaml_object, xaml_object>
{
};

xaml_result xaml_object_new(xaml_object** ptr) noexcept
{
    xaml_object* res = new (nothrow) xaml_object_impl();
    if (!res) return XAML_E_OUTOFMEMORY;
    *ptr = res;
    return XAML_S_OK;
}
