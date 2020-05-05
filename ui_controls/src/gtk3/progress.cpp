#include <shared/progress.hpp>
#include <xaml/ui/controls/progress.h>

using namespace std;

xaml_result xaml_progress_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        m_handle = gtk_progress_bar_new();
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_indeterminate());
    }
    return set_rect(region);
}

xaml_result xaml_progress_internal::draw_progress() noexcept
{
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(m_handle), (double)(m_value - m_minimum) / (double)(m_maximum - m_minimum));
    return XAML_S_OK;
}

xaml_result xaml_progress_internal::draw_indeterminate() noexcept
{
    if (m_is_indeterminate)
        return m_pulse_timer->start();
    else
        return draw_progress();
}

xaml_result xaml_progress_internal::on_pulse(xaml_ptr<xaml_timer>) noexcept
{
    if (m_is_indeterminate)
        gtk_progress_bar_pulse(GTK_PROGRESS_BAR(m_handle));
    else
        XAML_RETURN_IF_FAILED(m_pulse_timer->stop());
    return XAML_S_OK;
}
