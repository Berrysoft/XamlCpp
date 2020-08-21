#include <QProgressBar>
#include <shared/progress.hpp>

xaml_result xaml_progress_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        m_handle.reset(new QProgressBar());
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_indeterminate());
    }
    XAML_RETURN_IF_FAILED(set_rect(region));
    return XAML_S_OK;
}

xaml_result xaml_progress_internal::draw_progress() noexcept
{
    if (auto progress = m_handle.objectCast<QProgressBar>())
    {
        if (m_is_indeterminate)
        {
            progress->setRange(0, 0);
        }
        else
        {
            progress->setRange(m_minimum, m_maximum);
            progress->setValue(m_value);
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_progress_internal::draw_indeterminate() noexcept
{
    return draw_progress();
}
