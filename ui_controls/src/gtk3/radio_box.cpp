#include <shared/radio_box.hpp>
#include <xaml/ui/container.h>
#include <xaml/ui/controls/radio_box.h>

using namespace std;

xaml_result xaml_radio_box_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        m_handle = gtk_radio_button_new(nullptr);
        g_signal_connect(G_OBJECT(m_handle), "clicked", G_CALLBACK(xaml_button_internal::on_clicked), this);
        g_signal_connect(G_OBJECT(m_handle), "toggled", G_CALLBACK(xaml_radio_box_internal::on_toggled), this);
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_checked());
        XAML_RETURN_IF_FAILED(draw_group());
    }
    return set_rect(region);
}

xaml_result xaml_radio_box_internal::draw_checked() noexcept
{
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_handle), m_is_checked);
    return XAML_S_OK;
}

xaml_result xaml_radio_box_internal::draw_group() noexcept
{
    if (m_parent)
    {
        xaml_ptr<xaml_multicontainer> multic;
        if (XAML_SUCCEEDED(m_parent->query(&multic)))
        {
            xaml_ptr<xaml_vector_view> children;
            XAML_RETURN_IF_FAILED(multic->get_children(&children));
            XAML_FOREACH_START(c, children);
            {
                if (auto rc = c.query<xaml_radio_box>())
                {
                    if (rc.get() != static_cast<xaml_radio_box*>(m_outer_this))
                    {
                        xaml_ptr<xaml_string> group;
                        XAML_RETURN_IF_FAILED(rc->get_group(&group));
                        bool equals;
                        XAML_RETURN_IF_FAILED(group->equals(m_group.get(), &equals));
                        if (equals)
                        {
                            xaml_ptr<xaml_gtk3_control> native_control;
                            XAML_RETURN_IF_FAILED(rc->query(&native_control));
                            GtkWidget* native_handle;
                            XAML_RETURN_IF_FAILED(native_control->get_handle(&native_handle));
                            gtk_radio_button_join_group(GTK_RADIO_BUTTON(m_handle), GTK_RADIO_BUTTON(native_handle));
                            break;
                        }
                    }
                }
            }
            XAML_FOREACH_END();
        }
    }
    return XAML_S_OK;
}

void xaml_radio_box_internal::on_toggled(GtkWidget*, xaml_radio_box_internal* self) noexcept
{
    XAML_ASSERT_SUCCESS(self->set_is_checked(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self->m_handle))));
}
