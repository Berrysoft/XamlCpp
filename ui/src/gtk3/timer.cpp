#include <xaml/ui/timer.hpp>

using namespace std;
using namespace std::chrono;

namespace xaml
{
    gboolean timer::on_timeout(gpointer data)
    {
        timer* self = (timer*)data;
        self->m_tick(*self);
        return self->m_enabled ? TRUE : FALSE;
    }

    void timer::start()
    {
        if (!m_enabled.exchange(true))
        {
            g_timeout_add(m_interval.count(), G_SOURCE_FUNC(timer::on_timeout), this);
        }
    }

    void timer::stop()
    {
        m_enabled = false;
    }
} // namespace xaml
