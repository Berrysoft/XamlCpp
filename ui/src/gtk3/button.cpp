#include <xaml/ui/button.hpp>
#include <cmath>

using namespace std;

namespace xaml
{
    button::button() : common_control()
    {
        add_text_changed([this](button const&, string_view_t t) { if(get_handle()) gtk_button_set_label(GTK_BUTTON(get_handle()), t.data()); });
    }

    button::~button()
    {
    }

    void button::draw(rectangle const& region)
    {
        if (!get_handle())
        {
            set_handle(gtk_button_new());
            g_signal_connect(get_handle(), "clicked", G_CALLBACK(button::on_clicked), this);
        }
        rectangle real = region - get_margin();
        gtk_widget_set_size_request(get_handle(), (int)round(real.width), (int)round(real.height));
        gtk_button_set_label(GTK_BUTTON(get_handle()), m_text.c_str());
    }

    void button::on_clicked(GtkButton*, gpointer data)
    {
        button* s = (button*)data;
        s->m_click(*s);
    }
} // namespace xaml
