#include <QPushButton>
#include <qt5/qstring.hpp>
#include <shared/button.hpp>

xaml_result xaml_button_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        m_handle.reset(new QPushButton);
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_default());
    }
    XAML_RETURN_IF_FAILED(set_rect(region));
    return XAML_S_OK;
}

xaml_result xaml_button_internal::draw_text() noexcept
{
    if (auto button = qobject_cast<QAbstractButton*>(m_handle.get()))
    {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_text, &text));
        button->setText(text);
    }
    return XAML_S_OK;
}

xaml_result xaml_button_internal::draw_default() noexcept
{
    if (auto button = qobject_cast<QPushButton*>(m_handle.get()))
    {
        button->setDefault(m_is_default);
    }
    return XAML_S_OK;
}
