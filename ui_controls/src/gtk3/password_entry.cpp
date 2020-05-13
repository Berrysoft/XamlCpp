#include <shared/password_entry.hpp>
#include <xaml/ui/controls/password_entry.h>

xaml_result xaml_password_entry_internal::draw(xaml_rectangle const& region) noexcept
{
    XAML_RETURN_IF_FAILED(xaml_entry_internal::draw(region));
    gtk_entry_set_visibility(GTK_ENTRY(m_handle), FALSE);
    return draw_password_char();
}

xaml_result xaml_password_entry_internal::draw_password_char() noexcept
{
    if (m_password_char)
        gtk_entry_set_invisible_char(GTK_ENTRY(m_handle), m_password_char);
    else
        gtk_entry_unset_invisible_char(GTK_ENTRY(m_handle));
    return XAML_S_OK;
}
