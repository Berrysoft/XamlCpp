#include <QLineEdit>
#include <shared/password_entry.hpp>

xaml_result xaml_password_entry_internal::draw(xaml_rectangle const& region) noexcept
{
    bool new_handle = !m_handle;
    XAML_RETURN_IF_FAILED(xaml_entry_internal::draw(region));
    if (new_handle) XAML_RETURN_IF_FAILED(draw_password_char());
    return XAML_S_OK;
}

xaml_result xaml_password_entry_internal::draw_password_char() noexcept
{
    if (auto edit = m_handle.objectCast<QLineEdit>())
    {
        edit->setEchoMode(QLineEdit::Password);
    }
    return XAML_S_OK;
}
