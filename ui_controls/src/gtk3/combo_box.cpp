#include <shared/combo_box.hpp>
#include <xaml/ui/controls/combo_box.h>
#include <xaml/ui/drawing_conv.hpp>

using namespace std;

xaml_result xaml_combo_box_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        XAML_RETURN_IF_FAILED(draw_editable());
    }
    return set_rect(region);
}

xaml_result xaml_combo_box_internal::draw_text() noexcept
{
    if (m_is_editable)
    {
        auto entry = gtk_bin_get_child(GTK_BIN(m_handle));
        if (m_text)
        {
            char const* data;
            XAML_RETURN_IF_FAILED(m_text->get_data(&data));
            gtk_entry_set_text(GTK_ENTRY(entry), data);
        }
        else
        {
            int32_t size;
            XAML_RETURN_IF_FAILED(m_items->get_size(&size));
            if (m_sel_id < 0 || m_sel_id >= size)
            {
                gtk_entry_set_text(GTK_ENTRY(entry), "");
            }
            else
            {
                xaml_ptr<xaml_object> item;
                XAML_RETURN_IF_FAILED(m_items->get_at(m_sel_id, &item));
                if (auto str = item.query<xaml_string>())
                {
                    char const* data;
                    XAML_RETURN_IF_FAILED(str->get_data(&data));
                    gtk_entry_set_text(GTK_ENTRY(entry), data);
                }
            }
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::draw_items() noexcept
{
    XAML_FOREACH_START(item, m_items);
    {
        xaml_ptr<xaml_string> s = item.query<xaml_string>();
        if (s)
        {
            char const* data;
            XAML_RETURN_IF_FAILED(s->get_data(&data));
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(m_handle), data);
        }
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::draw_sel() noexcept
{
    gtk_combo_box_set_active(GTK_COMBO_BOX(m_handle), m_sel_id);
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::draw_editable() noexcept
{
    if (m_is_editable)
    {
        m_handle = gtk_combo_box_text_new_with_entry();
    }
    else
    {
        m_handle = gtk_combo_box_text_new();
    }
    g_signal_connect(G_OBJECT(m_handle), "changed", G_CALLBACK(xaml_combo_box_internal::on_changed), this);
    XAML_RETURN_IF_FAILED(draw_visible());
    XAML_RETURN_IF_FAILED(draw_items());
    XAML_RETURN_IF_FAILED(draw_sel());
    XAML_RETURN_IF_FAILED(draw_text());
    return XAML_S_OK;
}

void xaml_combo_box_internal::on_changed(GtkWidget*, xaml_combo_box_internal* self) noexcept
{
    XAML_ASSERT_SUCCEEDED(self->set_sel_id(gtk_combo_box_get_active(GTK_COMBO_BOX(self->m_handle))));
    if (self->m_is_editable)
    {
        auto entry = gtk_bin_get_child(GTK_BIN(self->m_handle));
        xaml_ptr<xaml_string> text;
        XAML_ASSERT_SUCCEEDED(xaml_string_new(gtk_entry_get_text(GTK_ENTRY(entry)), &text));
        XAML_ASSERT_SUCCEEDED(self->set_text(text));
    }
    else
    {
        int32_t size;
        XAML_ASSERT_SUCCEEDED(self->m_items->get_size(&size));
        if (self->m_sel_id < 0 || self->m_sel_id >= size)
        {
            XAML_ASSERT_SUCCEEDED(self->set_text(nullptr));
        }
        else
        {
            xaml_ptr<xaml_object> item;
            XAML_ASSERT_SUCCEEDED(self->m_items->get_at(self->m_sel_id, &item));
            if (auto str = item.query<xaml_string>())
            {
                char const* data;
                XAML_ASSERT_SUCCEEDED(str->get_data(&data));
                xaml_ptr<xaml_string> text;
                XAML_ASSERT_SUCCEEDED(xaml_string_new(data, &text));
                XAML_ASSERT_SUCCEEDED(self->set_text(text));
            }
        }
    }
}

xaml_result xaml_combo_box_internal::insert_item(int32_t index, xaml_ptr<xaml_object> const& value) noexcept
{
    xaml_ptr<xaml_string> s = value.query<xaml_string>();
    if (s)
    {
        char const* data;
        XAML_RETURN_IF_FAILED(s->get_data(&data));
        gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(m_handle), index, data);
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::remove_item(int32_t index) noexcept
{
    gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(m_handle), index);
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::clear_items() noexcept
{
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(m_handle));
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::replace_item(int32_t index, xaml_ptr<xaml_object> const& value) noexcept
{
    XAML_RETURN_IF_FAILED(remove_item(index));
    XAML_RETURN_IF_FAILED(insert_item(index, value));
    return XAML_S_OK;
}
