#include <xaml/ui/controls/check_box.hpp>
#include <xaml/ui/native_control.hpp>

using namespace std;

namespace xaml
{
    void check_box::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            auto h = make_shared<native_control>();
            h->handle = gtk_check_button_new();
            set_handle(h);
            g_signal_connect(G_OBJECT(get_handle()->handle), "clicked", G_CALLBACK(button::on_clicked), this);
            g_signal_connect(G_OBJECT(get_handle()->handle), "toggled", G_CALLBACK(check_box::on_toggled), this);
        }
        button::__draw(region);
        draw_checked();
    }

    void check_box::draw_checked()
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(get_handle()->handle), m_is_checked ? TRUE : FALSE);
    }

    void check_box::on_toggled(void*, void* data)
    {
        check_box* s = (check_box*)data;
        s->set_is_checked(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(s->get_handle()->handle)));
    }
} // namespace xaml
