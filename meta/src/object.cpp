#include <atomic>
#include <xaml/object.hpp>

using namespace std;

struct xaml_object_impl : xaml_implement<xaml_object_impl, xaml_object>
{
};

xaml_result xaml_object_new(xaml_object** ptr) noexcept
{
    return xaml_object_new<xaml_object_impl>(ptr);
}
