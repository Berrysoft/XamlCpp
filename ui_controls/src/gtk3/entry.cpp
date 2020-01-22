#include <gtk3/drawing.hpp>
#include <xaml/ui/controls/entry.hpp>

namespace xaml
{
    void entry::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            set_handle(gtk_entry_new());
            g_signal_connect(G_OBJECT(get_handle()), "changed", G_CALLBACK(entry::on_changed), this);
        }
        rectangle real = region - get_margin();
        __set_size_noevent({ real.width, real.height });
        draw_size();
        draw_text();
        draw_alignment();
    }

    void entry::draw_size()
    {
        gtk_widget_set_size_request(get_handle(), get_rwidth(get_width()), get_rheight(get_height()));
    }

    void entry::draw_text()
    {
        gtk_entry_set_text(GTK_ENTRY(get_handle()), m_text.c_str());
    }

    void entry::draw_alignment()
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
        gtk_entry_set_alignment(GTK_ENTRY(get_handle()), align);
    }

    void entry::on_changed(GtkEditable*, gpointer data)
    {
        entry* self = (entry*)data;
        self->set_text(gtk_entry_get_text(GTK_ENTRY(self->get_handle())));
    }

    void entry::__size_to_fit()
    {
        gtk_entry_set_width_chars(GTK_ENTRY(get_handle()), (gint)m_text.length());
        int width = gtk_widget_get_allocated_width(get_handle());
        int height = gtk_widget_get_allocated_height(get_handle());
        __set_size_noevent({ (double)width, (double)height });
    }
} // namespace xaml
