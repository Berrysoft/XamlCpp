#include <QCheckBox>
#include <shared/check_box.hpp>

xaml_result xaml_check_box_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        auto button = new QCheckBox();
        m_handle.reset(button);
        QObject::connect(
            button, &QAbstractButton::clicked,
            xaml_mem_fn(
                &xaml_button_internal::on_clicked,
                static_cast<xaml_button_internal*>(this)));
        QObject::connect(
            button, &QCheckBox::toggled,
            xaml_mem_fn(&xaml_check_box_internal::on_toggled, this));
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
    if (auto button = m_handle.objectCast<QCheckBox>())
    {
        button->setCheckState(m_is_checked ? Qt::Checked : Qt::Unchecked);
    }
    return XAML_S_OK;
}

void xaml_check_box_internal::on_toggled(bool checked) noexcept
{
    XAML_ASSERT_SUCCEEDED(set_is_checked(checked));
}
