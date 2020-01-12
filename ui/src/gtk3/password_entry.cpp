#include <gtk3/drawing.hpp>
#include <xaml/ui/password_entry.hpp>

namespace xaml
{
    void password_entry::__draw(const rectangle& region)
    {
        if (!get_handle())
        {
            set_handle(gtk_entry_new());
        }
        rectangle real = region - get_margin();
        set_size({ real.width, real.height });
        draw_text();
        gtk_entry_set_visibility(GTK_ENTRY(get_handle()), FALSE);
        draw_password_char();
    }

    void password_entry::draw_size()
    {
        gtk_widget_set_size_request(get_handle(), get_rwidth(get_width()), get_rheight(get_height()));
    }

    void password_entry::draw_text()
    {
        gtk_entry_set_text(GTK_ENTRY(get_handle()), m_text.c_str());
    }

    void password_entry::draw_password_char()
    {
        if (m_password_char)
            gtk_entry_set_invisible_char(GTK_ENTRY(get_handle()), m_password_char);
        else
            gtk_entry_unset_invisible_char(GTK_ENTRY(get_handle()));
    }

    void password_entry::__size_to_fit()
    {
        int width = gtk_widget_get_allocated_width(get_handle());
        int height = gtk_widget_get_allocated_height(get_handle());
        __set_size_noevent({ (double)width, (double)height });
    }
} // namespace xaml
