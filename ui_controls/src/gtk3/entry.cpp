#include <shared/entry.hpp>
#include <xaml/ui/controls/entry.h>

using namespace std;

xaml_result xaml_entry_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        m_handle = gtk_entry_new();
        g_signal_connect(G_OBJECT(m_handle), "changed", G_CALLBACK(xaml_entry_internal::on_changed), this);
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_alignment());
    }
    return set_rect(region);
}

xaml_result xaml_entry_internal::draw_text() noexcept
{
    xaml_char_t const* data = nullptr;
    if (m_text)
    {
        XAML_RETURN_IF_FAILED(m_text->get_data(&data));
    }
    gtk_entry_set_text(GTK_ENTRY(m_handle), data);
    return XAML_S_OK;
}

xaml_result xaml_entry_internal::draw_alignment() noexcept
{
    gfloat align;
    switch (m_text_halignment)
    {
    case xaml_halignment_center:
        align = 0.5;
        break;
    case xaml_halignment_right:
        align = 1.0;
        break;
    default:
        align = 0;
        break;
    }
    gtk_entry_set_alignment(GTK_ENTRY(m_handle), align);
    return XAML_S_OK;
}

void xaml_entry_internal::on_changed(GtkWidget* widget, xaml_entry_internal* self) noexcept
{
    gchar const* data = gtk_entry_get_text(GTK_ENTRY(self->m_handle));
    xaml_ptr<xaml_string> text;
    XAML_ASSERT_SUCCEEDED(xaml_string_new(data, &text));
    XAML_ASSERT_SUCCEEDED(self->set_text(text.get()));
}

xaml_result xaml_entry_internal::size_to_fit() noexcept
{
    int32_t length;
    XAML_RETURN_IF_FAILED(m_text->get_length(&length));
    gtk_entry_set_width_chars(GTK_ENTRY(m_handle), length);
    return xaml_control_internal::size_to_fit();
}
