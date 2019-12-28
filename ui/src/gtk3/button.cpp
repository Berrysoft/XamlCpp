#include <internal/gtk3/drawing.hpp>
#include <xaml/ui/button.hpp>

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

    button::~button()
    {
    }

    void button::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            set_handle(gtk_button_new());
            g_signal_connect(get_handle(), "clicked", G_CALLBACK(button::on_clicked), this);
        }
        rectangle real = region - get_margin();
        set_size({ real.width, real.height });
        draw_text();
    }

    void button::draw_text()
    {
        gtk_button_set_label(GTK_BUTTON(get_handle()), m_text.c_str());
    }

    void button::draw_size()
    {
        gtk_widget_set_size_request(get_handle(), get_rwidth(get_width()), get_rheight(get_height()));
    }

    void button::draw_default() {}

    void button::on_clicked(GtkButton*, gpointer data)
    {
        button* s = (button*)data;
        s->m_click(*s);
    }
} // namespace xaml
