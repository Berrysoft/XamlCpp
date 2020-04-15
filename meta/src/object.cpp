#include <atomic>
#include <xaml/object.hpp>

using namespace std;

struct xaml_object_impl : xaml_implement<xaml_object_impl, xaml_object, xaml_object>
{
};

xaml_object* xaml_object_new() XAML_NOEXCEPT
{
    return new xaml_object_impl();
}
