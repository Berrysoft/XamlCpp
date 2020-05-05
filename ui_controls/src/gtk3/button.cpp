#include <shared/button.hpp>
#include <xaml/ui/controls/button.h>

using namespace std;

xaml_result xaml_button_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        m_handle = gtk_button_new();
        g_signal_connect(G_OBJECT(m_handle), "clicked", G_CALLBACK(xaml_button_internal::on_clicked), this);
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
    }
    return set_rect(region);
}

xaml_result xaml_button_internal::draw_text() noexcept
{
    xaml_char_t const* data = nullptr;
    if (m_text)
    {
        XAML_RETURN_IF_FAILED(m_text->get_data(&data));
    }
    gtk_button_set_label(GTK_BUTTON(m_handle), data);
    return XAML_S_OK;
}

xaml_result xaml_button_internal::draw_default() noexcept { return XAML_S_OK; }

void xaml_button_internal::on_clicked(GtkWidget*, xaml_button_internal* self) noexcept
{
    XAML_ASSERT_SUCCEEDED(self->on_click(self->m_outer_this));
}
