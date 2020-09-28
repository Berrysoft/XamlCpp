#include <xaml/event.h>

struct xaml_event_args_impl : xaml_implement<xaml_event_args_impl, xaml_event_args>
{
};

xaml_event_args_impl m_empty_instance;

xaml_result XAML_CALL xaml_event_args_empty(xaml_event_args** ptr) noexcept
{
    m_empty_instance.add_ref();
    *ptr = &m_empty_instance;
    return XAML_S_OK;
}
