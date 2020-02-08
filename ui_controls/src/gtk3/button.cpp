#include <xaml/ui/controls/button.hpp>

using namespace std;

namespace xaml
{
    button::button() : common_control()
    {
        add_text_changed([this](button const&, string_view_t) {
            if (get_handle())
            {
                draw_text();
                __parent_redraw();
            }
        });
        add_size_changed([this](control const&, size) {
            if (get_handle())
            {
                draw_size();
                __parent_redraw();
            }
        });
    }

    button::~button() {}

    void button::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            set_handle(gtk_button_new());
            g_signal_connect(get_handle(), "clicked", G_CALLBACK(button::on_clicked), this);
        }
        rectangle real = region - get_margin();
        __set_size_noevent({ real.width, real.height });
        draw_size();
        draw_text();
    }

    void button::draw_text()
    {
        gtk_button_set_label(GTK_BUTTON(get_handle()), m_text.c_str());
    }

    void button::draw_size()
    {
        auto [rw, rh] = to_native<tuple<gint, gint>>(get_size());
        gtk_widget_set_size_request(get_handle(), rw, rh);
    }

    void button::draw_default() {}

    void button::on_clicked(GtkButton*, gpointer data)
    {
        button* s = (button*)data;
        s->m_click(*s);
    }

    void button::__size_to_fit()
    {
        int width = gtk_widget_get_allocated_width(get_handle());
        int height = gtk_widget_get_allocated_height(get_handle());
        __set_size_noevent({ (double)width, (double)height });
    }
} // namespace xaml
