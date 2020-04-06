#include <xaml/ui/controls/progress.hpp>
#include <xaml/ui/native_control.hpp>

using namespace std;

namespace xaml
{
    void progress::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            auto h = make_shared<native_control>();
            h->handle = gtk_progress_bar_new();
            set_handle(h);
            m_pulse_timer.set_interval(100ms);
            m_pulse_timer.add_tick(mem_fn_bind(&progress::on_pulse, this));
            draw_visible();
            draw_indeterminate();
        }
        __set_rect(region);
    }

    void progress::draw_progress()
    {
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(get_handle()->handle), (double)(m_value - m_minimum) / (double)(m_maximum - m_minimum));
    }

    void progress::draw_indeterminate()
    {
        if (m_is_indeterminate)
            m_pulse_timer.start();
        else
            draw_progress();
    }

    void progress::on_pulse(timer&)
    {
        if (m_is_indeterminate)
            gtk_progress_bar_pulse(GTK_PROGRESS_BAR(get_handle()->handle));
        else
            m_pulse_timer.stop();
    }
} // namespace xaml
