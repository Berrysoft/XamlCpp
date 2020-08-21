#include <QLabel>
#include <qt5/qstring.hpp>
#include <shared/label.hpp>

xaml_result xaml_label_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        m_handle.reset(new QLabel);
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_alignment());
    }
    return set_rect(region);
}

xaml_result xaml_label_internal::draw_text() noexcept
{
    if (auto label = m_handle.objectCast<QLabel>())
    {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_text, &text));
        label->setText(text);
    }
    return XAML_S_OK;
}

xaml_result xaml_label_internal::draw_alignment() noexcept
{
    if (auto label = m_handle.objectCast<QLabel>())
    {
        Qt::Alignment align;
        switch (m_text_halignment)
        {
        case xaml_halignment_center:
            align = Qt::AlignHCenter;
            break;
        case xaml_halignment_right:
            align = Qt::AlignRight;
            break;
        default:
            align = Qt::AlignLeft;
            break;
        }
        label->setAlignment(align);
    }
    return XAML_S_OK;
}
