#include <QCheckBox>
#include <shared/check_box.hpp>

xaml_result xaml_check_box_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        m_handle.reset(new QCheckBox);
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_default());
        XAML_RETURN_IF_FAILED(draw_checked());
    }
    XAML_RETURN_IF_FAILED(set_rect(region));
    return XAML_S_OK;
}

xaml_result xaml_check_box_internal::draw_checked() noexcept
{
    if (auto button = qobject_cast<QCheckBox*>(m_handle.get()))
    {
        button->setCheckState(m_is_checked ? Qt::Checked : Qt::Unchecked);
    }
    return XAML_S_OK;
}
