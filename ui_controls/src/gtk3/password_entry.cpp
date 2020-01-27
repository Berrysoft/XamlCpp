#include <xaml/ui/controls/password_entry.hpp>

namespace xaml
{
    void password_entry::__draw(const rectangle& region)
    {
        entry::__draw(region);
        gtk_entry_set_visibility(GTK_ENTRY(get_handle()), FALSE);
        draw_password_char();
    }

    void password_entry::draw_password_char()
    {
        if (m_password_char)
            gtk_entry_set_invisible_char(GTK_ENTRY(get_handle()), m_password_char);
        else
            gtk_entry_unset_invisible_char(GTK_ENTRY(get_handle()));
    }
} // namespace xaml
