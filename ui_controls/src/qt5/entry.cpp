#include <QLineEdit>
#include <qt5/qstring.hpp>
#include <shared/entry.hpp>

xaml_result xaml_entry_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        auto edit = new QLineEdit();
        m_handle.reset(edit);
        QObject::connect(
            edit, &QLineEdit::textChanged,
            xaml_mem_fn(&xaml_entry_internal::on_text_changed_event, this));
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_alignment());
    }
    XAML_RETURN_IF_FAILED(set_rect(region));
    return XAML_S_OK;
}

xaml_result xaml_entry_internal::draw_text() noexcept
{
    if (auto edit = m_handle.objectCast<QLineEdit>())
    {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_text, &text));
        edit->setText(text);
    }
    return XAML_S_OK;
}

xaml_result xaml_entry_internal::draw_alignment() noexcept
{
    if (auto edit = m_handle.objectCast<QLineEdit>())
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
        edit->setAlignment(align);
    }
    return XAML_S_OK;
}

void xaml_entry_internal::on_text_changed_event(QString const& text) noexcept
{
    xaml_ptr<xaml_string> str;
    XAML_ASSERT_SUCCEEDED(xaml_string_new(text, &str));
    XAML_ASSERT_SUCCEEDED(set_text(str));
}
