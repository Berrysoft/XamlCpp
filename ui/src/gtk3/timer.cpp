#include <gtk/gtk.h>
#include <xaml/ui/timer.hpp>

using namespace std;

namespace xaml
{
    int timer::on_timeout(void* data)
    {
        timer* self = (timer*)data;
        self->m_tick(*self);
        return self->m_enabled ? TRUE : FALSE;
    }

    void timer::start()
    {
        if (!m_enabled.exchange(true))
        {
            g_timeout_add((guint)m_interval.count(), timer::on_timeout, this);
        }
    }

    void timer::stop()
    {
        m_enabled = false;
    }
} // namespace xaml
