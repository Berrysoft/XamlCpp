#include <xaml/ui/container.hpp>
#include <xaml/ui/controls/radio_box.hpp>
#include <xaml/ui/native_control.hpp>

using namespace std;

namespace xaml
{
    void radio_box::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            auto h = make_shared<native_control>();
            h->handle = gtk_radio_button_new(nullptr);
            set_handle(h);
            g_signal_connect(G_OBJECT(get_handle()->handle), "clicked", G_CALLBACK(button::on_clicked), this);
            g_signal_connect(G_OBJECT(get_handle()->handle), "toggled", G_CALLBACK(radio_box::on_toggled), this);
        }
        button::__draw(region);
        draw_checked();
        draw_group();
    }

    void radio_box::draw_checked()
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(get_handle()->handle), m_is_checked ? TRUE : FALSE);
    }

    void radio_box::draw_group()
    {
        if (auto sparent = get_parent().lock())
        {
            if (auto multic = dynamic_pointer_cast<multicontainer>(sparent))
            {
                for (auto& c : multic->get_children())
                {
                    if (auto rc = dynamic_pointer_cast<radio_box>(c))
                    {
                        if (c != shared_from_this() && c->get_handle() && rc->get_group() == get_group())
                        {
                            gtk_radio_button_set_group(GTK_RADIO_BUTTON(get_handle()->handle), gtk_radio_button_get_group(GTK_RADIO_BUTTON(c->get_handle()->handle)));
                            break;
                        }
                    }
                }
            }
        }
    }

    void radio_box::on_toggled(void*, void* data)
    {
        radio_box* s = (radio_box*)data;
        s->set_is_checked(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(s->get_handle()->handle)));
    }
} // namespace xaml
