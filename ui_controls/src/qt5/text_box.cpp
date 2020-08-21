#include <QTextEdit>
#include <qt5/qstring.hpp>
#include <shared/text_box.hpp>

using namespace std;

xaml_result xaml_text_box_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        auto edit = new QTextEdit;
        m_handle.reset(edit);
        edit->setLineWrapMode(QTextEdit::WidgetWidth);
        edit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        QObject::connect(
            edit, &QTextEdit::textChanged,
            xaml_mem_fn(&xaml_text_box_internal::on_text_changed_event, this));
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
    }
    return set_rect(region);
}

xaml_result xaml_text_box_internal::draw_text() noexcept
{
    if (auto edit = m_handle.objectCast<QTextEdit>())
    {
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_text, &text));
        edit->setText(text);
    }
    return XAML_S_OK;
}

void xaml_text_box_internal::on_text_changed_event() noexcept
{
    if (auto edit = m_handle.objectCast<QTextEdit>())
    {
        xaml_ptr<xaml_string> str;
        XAML_ASSERT_SUCCEEDED(xaml_string_new(edit->toPlainText(), &str));
        XAML_ASSERT_SUCCEEDED(set_text(str));
    }
}
