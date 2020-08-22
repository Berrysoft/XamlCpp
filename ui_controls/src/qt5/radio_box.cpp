#include <QRadioButton>
#include <shared/radio_box.hpp>

using namespace std;

xaml_result xaml_radio_box_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        m_handle = create<QRadioButton>();
        auto button = static_cast<QRadioButton*>(m_handle);
        QObject::connect(
            button, &QAbstractButton::clicked,
            xaml_mem_fn(
                &xaml_button_internal::on_clicked,
                static_cast<xaml_button_internal*>(this)));
        QObject::connect(
            button, &QRadioButton::toggled,
            xaml_mem_fn(&xaml_radio_box_internal::on_toggled, this));
        button->setAutoExclusive(false);
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_default());
        XAML_RETURN_IF_FAILED(draw_group());
        XAML_RETURN_IF_FAILED(draw_checked());
    }
    return set_rect(region);
}

xaml_result xaml_radio_box_internal::draw_checked() noexcept
{
    if (auto button = qobject_cast<QRadioButton*>(m_handle))
    {
        button->setChecked(m_is_checked);
    }
    return XAML_S_OK;
}

void xaml_radio_box_internal::on_toggled(bool checked) noexcept
{
    XAML_ASSERT_SUCCEEDED(set_is_checked(checked));
}
