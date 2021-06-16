#include <QPushButton>
#include <qt/qstring.hpp>
#include <shared/button.hpp>

using namespace std;

xaml_result xaml_button_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        XAML_RETURN_IF_FAILED(create<QPushButton>());
        auto button = static_cast<QPushButton*>(m_handle);
        QObject::connect(
            button, &QAbstractButton::clicked,
            xaml_mem_fn(&xaml_button_internal::on_clicked, this));
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_default());
    }
    return set_rect(region);
}

xaml_result xaml_button_internal::draw_text() noexcept
{
    if (auto button = qobject_cast<QAbstractButton*>(m_handle))
    {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_text, &text));
        button->setText(text);
    }
    return XAML_S_OK;
}

xaml_result xaml_button_internal::draw_default() noexcept
{
    if (auto button = qobject_cast<QPushButton*>(m_handle))
    {
        button->setDefault(m_is_default);
    }
    return XAML_S_OK;
}

void xaml_button_internal::on_clicked(bool) noexcept
{
    xaml_ptr<xaml_event_args> args;
    XAML_ASSERT_SUCCEEDED(xaml_event_args_empty(&args));
    XAML_ASSERT_SUCCEEDED(m_click->invoke(m_outer_this, args));
}
