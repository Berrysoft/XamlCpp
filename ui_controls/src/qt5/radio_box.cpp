#include <QRadioButton>
#include <shared/radio_box.hpp>

xaml_result xaml_radio_box_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        m_handle.reset(new QRadioButton);
        static_cast<QRadioButton*>(m_handle.get())->setAutoExclusive(false);
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_default());
        XAML_RETURN_IF_FAILED(draw_group());
        XAML_RETURN_IF_FAILED(draw_checked());
    }
    XAML_RETURN_IF_FAILED(set_rect(region));
    return XAML_S_OK;
}

xaml_result xaml_radio_box_internal::draw_checked() noexcept
{
    if (auto button = qobject_cast<QRadioButton*>(m_handle.get()))
    {
        button->setChecked(m_is_checked);
    }
    return XAML_S_OK;
}
