#include <xaml/ui/controls/check_box.hpp>

namespace xaml
{
    void check_box::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            set_handle(gtk_check_button_new());
            g_signal_connect(G_OBJECT(get_handle()), "clicked", G_CALLBACK(button::on_clicked), this);
            g_signal_connect(G_OBJECT(get_handle()), "toggled", G_CALLBACK(check_box::on_toggled), this);
        }
        button::__draw(region);
        draw_checked();
    }

    void check_box::draw_checked()
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(get_handle()), m_is_checked ? TRUE : FALSE);
    }

    void check_box::on_toggled(GtkToggleButton*, gpointer data)
    {
        check_box* s = (check_box*)data;
        s->set_is_checked(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(s->get_handle())));
    }
} // namespace xaml
