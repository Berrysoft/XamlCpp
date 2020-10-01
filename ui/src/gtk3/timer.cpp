#include <gtk/gtk.h>
#include <shared/timer.hpp>
#include <xaml/ui/timer.h>

using namespace std;

gboolean xaml_timer_impl::on_timeout(xaml_timer_impl* self) noexcept
{
    xaml_ptr<xaml_event_args> args;
    XAML_ASSERT_SUCCEEDED(xaml_event_args_empty(&args));
    XAML_ASSERT_SUCCEEDED(self->m_tick->invoke(self, args));
    return self->m_is_enabled;
}

xaml_result xaml_timer_impl::start() noexcept
{
    if (!m_is_enabled.exchange(true))
    {
        g_timeout_add((guint)m_interval, (GSourceFunc)xaml_timer_impl::on_timeout, this);
    }
    return XAML_S_OK;
}

xaml_result xaml_timer_impl::stop() noexcept
{
    m_is_enabled = false;
    return XAML_S_OK;
}
