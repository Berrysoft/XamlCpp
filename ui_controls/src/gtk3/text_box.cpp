#include <gtk3/gstring.hpp>
#include <gtk3/resources.hpp>
#include <shared/atomic_guard.hpp>
#include <shared/text_box.hpp>

using namespace std;

xaml_result xaml_text_box_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        m_handle = gtk_scrolled_window_new(NULL, NULL);
        GtkWidget* text_view = gtk_text_view_new();
        gtk_container_add(GTK_CONTAINER(m_handle), text_view);
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
        GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        g_signal_connect(G_OBJECT(buffer), "changed", G_CALLBACK(xaml_text_box_internal::on_changed), this);
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
    }
    return set_rect(region);
}

xaml_result xaml_text_box_internal::draw_text() noexcept
{
    char const* data = nullptr;
    int32_t len = 0;
    if (m_text)
    {
        XAML_RETURN_IF_FAILED(m_text->get_data(&data));
        XAML_RETURN_IF_FAILED(m_text->get_length(&len));
    }
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_bin_get_child(GTK_BIN(m_handle))));
    gtk_text_buffer_set_text(buffer, data, len);
    return XAML_S_OK;
}

void xaml_text_box_internal::on_changed(GtkTextBuffer*, xaml_text_box_internal* self) noexcept
{
    xaml_atomic_guard guard{ self->m_text_changing };
    guard.test_and_set();
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_bin_get_child(GTK_BIN(self->m_handle))));
    GtkTextIter begin, end;
    gtk_text_buffer_get_start_iter(buffer, &begin);
    gtk_text_buffer_get_end_iter(buffer, &end);
    g_free_unique_ptr<gchar> data{ gtk_text_buffer_get_text(buffer, &begin, &end, TRUE) };
    xaml_ptr<xaml_string> text;
    XAML_ASSERT_SUCCEEDED(xaml_string_new_gstring(move(data), &text));
    XAML_ASSERT_SUCCEEDED(self->set_text(text));
}
