#include <xaml/ui/controls/label.hpp>

using namespace std;

namespace xaml
{
    void label::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            set_handle(gtk_label_new(m_text.c_str()));
        }
        rectangle real = region - get_margin();
        __set_size_noevent({ real.width, real.height });
        draw_size();
        draw_text();
        draw_alignment();
    }

    void label::draw_size()
    {
        auto [rw, rh] = to_native<tuple<gint, gint>>(get_size());
        gtk_widget_set_size_request(get_handle(), rw, rh);
    }

    void label::draw_text()
    {
        gtk_label_set_label(GTK_LABEL(get_handle()), m_text.c_str());
    }

    void label::draw_alignment()
    {
        gfloat align;
        switch (m_text_halignment)
        {
        case halignment_t::center:
            align = 0.5;
            break;
        case halignment_t::right:
            align = 1.0;
            break;
        default:
            align = 0;
            break;
        }
        gtk_label_set_xalign(GTK_LABEL(get_handle()), align);
    }
} // namespace xaml
