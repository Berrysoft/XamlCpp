#include <QPushButton>
#include <qt5/qstring.hpp>
#include <shared/button.hpp>

xaml_result xaml_button_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        m_handle = create<QPushButton>();
        auto button = m_handle.staticCast<QPushButton>();
        QObject::connect(
            button.get(), &QAbstractButton::clicked,
            xaml_mem_fn(&xaml_button_internal::on_clicked, this));
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_default());
    }
    return set_rect(region);
}

xaml_result xaml_button_internal::draw_text() noexcept
{
    if (auto button = m_handle.objectCast<QAbstractButton>())
    {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_text, &text));
        button->setText(text);
    }
    return XAML_S_OK;
}

xaml_result xaml_button_internal::draw_default() noexcept
{
    if (auto button = m_handle.objectCast<QPushButton>())
    {
        button->setDefault(m_is_default);
    }
    return XAML_S_OK;
}

void xaml_button_internal::on_clicked(bool) noexcept
{
    XAML_ASSERT_SUCCEEDED(on_click(m_outer_this));
}
