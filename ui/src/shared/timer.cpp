#include <shared/timer.hpp>
#include <xaml/ui/timer.h>

using namespace std;

xaml_result xaml_timer_impl::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_tick));
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_timer_new(xaml_timer** ptr) noexcept
{
    return xaml_object_new_and_init<xaml_timer_impl>(ptr);
}

xaml_result XAML_CALL xaml_timer_new_interval(int32_t interval, xaml_timer** ptr) noexcept
{
    return xaml_object_new_and_init<xaml_timer_impl>(ptr, interval);
}
