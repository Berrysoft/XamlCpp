#include <shared/timer.hpp>
#include <xaml/ui/timer.h>

using namespace std;

xaml_result XAML_CALL xaml_timer_new(xaml_timer** ptr) noexcept
{
    return xaml_object_new<xaml_timer_impl>(ptr);
}

xaml_result XAML_CALL xaml_timer_new_interval(int32_t interval, xaml_timer** ptr) noexcept
{
    return xaml_object_new<xaml_timer_impl>(ptr, interval);
}
