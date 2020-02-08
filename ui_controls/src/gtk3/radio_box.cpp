#include <xaml/ui/controls/radio_box.hpp>

namespace xaml
{
    void radio_box::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            set_handle(gtk_check_button_new());
            g_signal_connect(get_handle(), "clicked", G_CALLBACK(button::on_clicked), this);
            g_signal_connect(get_handle(), "toggled", G_CALLBACK(radio_box::on_toggled), this);
        }
        button::__draw(region);
        draw_checked();
    }

    void radio_box::draw_checked()
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(get_handle()), m_is_checked ? TRUE : FALSE);
    }

    void radio_box::on_toggled(GtkToggleButton*, gpointer data)
    {
        radio_box* s = (radio_box*)data;
        s->set_is_checked(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(s->get_handle())));
    }
} // namespace xaml
