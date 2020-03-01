#include <xaml/ui/controls/label.hpp>
#include <xaml/ui/native_control.hpp>

using namespace std;

namespace xaml
{
    void label::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            auto h = make_shared<native_control>();
            h->handle = gtk_label_new(m_text.c_str());
            set_handle(h);
        }
        rectangle real = region - get_margin();
        __set_size_noevent({ real.width, real.height });
        draw_size();
        draw_text();
        draw_alignment();
    }

    void label::draw_text()
    {
        gtk_label_set_label(GTK_LABEL(get_handle()->handle), m_text.c_str());
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
        gtk_label_set_xalign(GTK_LABEL(get_handle()->handle), align);
    }
} // namespace xaml
