#include <xaml/ui/controls/combo_box.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_drawing.hpp>

using namespace std;

namespace xaml
{
    void combo_box::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            draw_editable();
        }
        __set_rect(region);
    }

    void combo_box::draw_text()
    {
        if (m_is_editable)
        {
            auto entry = gtk_bin_get_child(GTK_BIN(get_handle()->handle));
            if (m_text)
            {
                gtk_entry_set_text(GTK_ENTRY(entry), m_text->get().data());
            }
            else
            {
                if (get_sel_id() < 0 || get_sel_id() >= get_items().size())
                {
                    gtk_entry_set_text(GTK_ENTRY(entry), "");
                }
                else
                {
                    gtk_entry_set_text(GTK_ENTRY(entry), get_items()[get_sel_id()].get().c_str());
                }
            }
        }
    }

    void combo_box::draw_items()
    {
        for (auto item : get_items())
        {
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(get_handle()->handle), item.get().c_str());
        }
    }

    void combo_box::draw_sel()
    {
        gtk_combo_box_set_active(GTK_COMBO_BOX(get_handle()->handle), (gint)get_sel_id());
    }

    void combo_box::draw_editable()
    {
        auto h = make_shared<native_control>();
        if (m_is_editable)
        {
            h->handle = gtk_combo_box_text_new_with_entry();
        }
        else
        {
            h->handle = gtk_combo_box_text_new();
        }
        set_handle(h);
        g_signal_connect(G_OBJECT(get_handle()->handle), "changed", G_CALLBACK(combo_box::on_changed), this);
        draw_visible();
        draw_items();
        draw_sel();
        draw_text();
    }

    void combo_box::on_changed(void* widget, void* data)
    {
        combo_box* self = (combo_box*)data;
        self->set_sel_id(gtk_combo_box_get_active(GTK_COMBO_BOX(self->get_handle()->handle)));
        if (self->get_is_editable())
        {
            auto entry = gtk_bin_get_child(GTK_BIN(self->get_handle()->handle));
            self->set_text(box_value(gtk_entry_get_text(GTK_ENTRY(entry))));
        }
        else if (self->get_sel_id() < 0 || self->get_sel_id() >= self->get_items().size())
        {
            self->set_text({});
        }
        else
        {
            self->set_text(box_value(self->get_items()[self->get_sel_id()].get()));
        }
    }

    void combo_box::insert_item(int32_t index, string_t const& value)
    {
        gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(get_handle()->handle), (gint)index, value.c_str());
    }

    void combo_box::remove_item(int32_t index)
    {
        gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(get_handle()->handle), (gint)index);
    }

    void combo_box::clear_items()
    {
        gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(get_handle()->handle));
    }

    void combo_box::replace_item(int32_t index, string_t const& value)
    {
        remove_item(index);
        insert_item(index, value);
    }
} // namespace xaml
