#include <shared/timer.hpp>

xaml_result xaml_timer_impl::start() noexcept
{
    if (!m_is_enabled.exchange(true))
    {
        m_handle.start(m_interval);
    }
    return XAML_S_OK;
}

xaml_result xaml_timer_impl::stop() noexcept
{
    if (m_is_enabled.exchange(false))
    {
        m_handle.stop();
    }
    return XAML_S_OK;
}
