#include <gtk3/drawing.hpp>
#include <xaml/ui/label.hpp>

using namespace std;

namespace xaml
{
    label::label() : common_control()
    {
        add_text_changed([this](label const&, string_view_t) {
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

    label::~label() {}

    void label::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            set_handle(gtk_label_new(m_text.c_str()));
        }
        rectangle real = region - get_margin();
        set_size({ real.width, real.height });
        draw_text();
        draw_alignment();
    }

    void label::draw_size()
    {
        gtk_widget_set_size_request(get_handle(), get_rwidth(get_width()), get_rheight(get_height()));
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

    void label::__size_to_fit()
    {
        int width = gtk_widget_get_allocated_width(get_handle());
        int height = gtk_widget_get_allocated_height(get_handle());
        __set_size_noevent({ (double)width, (double)height });
    }
} // namespace xaml
