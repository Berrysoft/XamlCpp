#include <shared/check_box.hpp>
#include <xaml/ui/controls/check_box.h>

using namespace std;

xaml_result xaml_check_box_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        m_handle = gtk_check_button_new();
        g_signal_connect(G_OBJECT(m_handle), "clicked", G_CALLBACK(xaml_button_internal::on_clicked), this);
        g_signal_connect(G_OBJECT(m_handle), "toggled", G_CALLBACK(xaml_check_box_internal::on_toggled), this);
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_checked());
    }
    return set_rect(region);
}

xaml_result xaml_check_box_internal::draw_checked() noexcept
{
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_handle), m_is_checked);
    return XAML_S_OK;
}

void xaml_check_box_internal::on_toggled(GtkWidget*, xaml_check_box_internal* self) noexcept
{
    XAML_ASSERT_SUCCESS(self->set_is_checked(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self->m_handle))));
}
