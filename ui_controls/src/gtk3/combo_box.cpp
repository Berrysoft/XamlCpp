#include <xaml/ui/controls/combo_box.hpp>

using namespace std;

namespace xaml
{
    void combo_box::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            draw_create();
        }
        rectangle real = region - get_margin();
        __set_size_noevent({ real.width, real.height });
        draw_sel();
        draw_editable();
    }

    void combo_box::draw_create()
    {
        if (m_is_editable)
        {
            set_handle(gtk_combo_box_text_new_with_entry());
        }
        else
        {
            set_handle(gtk_combo_box_text_new());
        }
        draw_items();
    }

    void combo_box::draw_size()
    {
        auto [rw, rh] = to_native<tuple<gint, gint>>(get_size());
        gtk_widget_set_size_request(get_handle(), rw, rh);
    }

    void combo_box::draw_text()
    {
        if (m_is_editable)
        {
            auto entry = gtk_bin_get_child(GTK_BIN(get_handle()));
            if (m_text)
            {
                gtk_entry_set_text(GTK_ENTRY(entry), m_text->c_str());
            }
            else
            {
                if (m_sel_id < 0 || m_sel_id >= m_items.size())
                {
                    gtk_entry_set_text(GTK_ENTRY(entry), "");
                }
                else
                {
                    gtk_entry_set_text(GTK_ENTRY(entry), m_items[m_sel_id].c_str());
                }
            }
        }
    }

    void combo_box::draw_items()
    {
        for (string_t const& item : m_items)
        {
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(get_handle()), item.c_str());
        }
    }

    void combo_box::draw_sel()
    {
        gtk_combo_box_set_active(GTK_COMBO_BOX(get_handle()), (gint)m_sel_id);
    }

    void combo_box::draw_editable()
    {
    }

    void combo_box::__size_to_fit()
    {
        int width = gtk_widget_get_allocated_width(get_handle());
        int height = gtk_widget_get_allocated_height(get_handle());
        __set_size_noevent({ (double)width, (double)height });
    }
} // namespace xaml
