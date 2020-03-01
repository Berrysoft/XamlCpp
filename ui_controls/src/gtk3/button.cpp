#include <xaml/ui/controls/button.hpp>
#include <xaml/ui/native_control.hpp>

using namespace std;

namespace xaml
{
    void button::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            auto h = make_shared<native_control>();
            h->handle = gtk_button_new();
            set_handle(h);
            g_signal_connect(G_OBJECT(get_handle()->handle), "clicked", G_CALLBACK(button::on_clicked), this);
        }
        rectangle real = region - get_margin();
        __set_size_noevent({ real.width, real.height });
        draw_size();
        draw_text();
    }

    void button::draw_text()
    {
        gtk_button_set_label(GTK_BUTTON(get_handle()->handle), m_text.c_str());
    }

    void button::draw_default() {}

    void button::on_clicked(void*, void* data)
    {
        button* s = (button*)data;
        s->m_click(*s);
    }
} // namespace xaml
