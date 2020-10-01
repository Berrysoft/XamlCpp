#include <shared/menu_item.hpp>
#include <xaml/ui/controls/menu_item.h>

using namespace std;

xaml_result xaml_menu_item_internal::draw(xaml_rectangle const&) noexcept
{
    if (!m_handle)
    {
        char const* data;
        XAML_RETURN_IF_FAILED(m_text->get_data(&data));
        m_handle = gtk_menu_item_new_with_label(data);
        g_signal_connect(G_OBJECT(m_handle), "activate", G_CALLBACK(xaml_menu_item_internal::on_activate), this);
        XAML_RETURN_IF_FAILED(draw_visible());
    }
    return XAML_S_OK;
}

void xaml_menu_item_internal::on_activate(GtkWidget*, xaml_menu_item_internal* self) noexcept
{
    xaml_ptr<xaml_event_args> args;
    XAML_ASSERT_SUCCEEDED(xaml_event_args_empty(&args));
    XAML_ASSERT_SUCCEEDED(self->m_click->invoke(self->m_outer_this, args));
}

xaml_result xaml_popup_menu_item_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        XAML_RETURN_IF_FAILED(xaml_menu_item_internal::draw(region));
        XAML_RETURN_IF_FAILED(draw_submenu());
    }
    return XAML_S_OK;
}

xaml_result xaml_popup_menu_item_internal::draw_submenu() noexcept
{
    GtkWidget* subm = gtk_menu_new();
    XAML_FOREACH_START(xaml_menu_item, cc, m_submenu);
    {
        XAML_RETURN_IF_FAILED(cc->draw({}));
        xaml_ptr<xaml_gtk3_control> native_control;
        XAML_RETURN_IF_FAILED(cc->query(&native_control));
        GtkWidget* native_handle;
        XAML_RETURN_IF_FAILED(native_control->get_handle(&native_handle));
        gtk_menu_shell_append(GTK_MENU_SHELL(subm), native_handle);
    }
    XAML_FOREACH_END();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(m_handle), subm);
    return XAML_S_OK;
}

xaml_result xaml_check_menu_item_internal::draw(xaml_rectangle const&) noexcept
{
    if (!m_handle)
    {
        char const* data;
        XAML_RETURN_IF_FAILED(m_text->get_data(&data));
        m_handle = gtk_check_menu_item_new_with_label(data);
        g_signal_connect(G_OBJECT(m_handle), "activate", G_CALLBACK(xaml_menu_item_internal::on_activate), this);
        {
            xaml_ptr<xaml_element_base> parent;
            XAML_RETURN_IF_FAILED(get_parent(&parent));
            xaml_ptr<xaml_gtk3_control> native_parent;
            XAML_RETURN_IF_FAILED(parent->query(&native_parent));
            GtkWidget* parent_handle;
            XAML_RETURN_IF_FAILED(native_parent->get_handle(&parent_handle));
            gtk_menu_shell_append(GTK_MENU_SHELL(parent_handle), m_handle);
        }
        XAML_RETURN_IF_FAILED(draw_checked());
    }
    return XAML_S_OK;
}

xaml_result xaml_check_menu_item_internal::draw_checked() noexcept
{
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(m_handle), m_is_checked);
    return XAML_S_OK;
}

xaml_result xaml_radio_menu_item_internal::draw(xaml_rectangle const&) noexcept
{
    if (!m_handle)
    {
        char const* data;
        XAML_RETURN_IF_FAILED(m_text->get_data(&data));
        m_handle = gtk_radio_menu_item_new_with_label(nullptr, data);
        g_signal_connect(G_OBJECT(m_handle), "activate", G_CALLBACK(xaml_menu_item_internal::on_activate), this);
        {
            xaml_ptr<xaml_element_base> parent;
            XAML_RETURN_IF_FAILED(get_parent(&parent));
            xaml_ptr<xaml_gtk3_control> native_parent;
            XAML_RETURN_IF_FAILED(parent->query(&native_parent));
            GtkWidget* parent_handle;
            XAML_RETURN_IF_FAILED(native_parent->get_handle(&parent_handle));
            gtk_menu_shell_append(GTK_MENU_SHELL(parent_handle), m_handle);
        }
        XAML_RETURN_IF_FAILED(draw_checked());
        XAML_RETURN_IF_FAILED(draw_group());
    }
    return XAML_S_OK;
}

xaml_result xaml_radio_menu_item_internal::draw_checked() noexcept
{
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(m_handle), m_is_checked);
    return XAML_S_OK;
}

xaml_result xaml_radio_menu_item_internal::draw_group() noexcept
{
    xaml_ptr<xaml_element_base> parent;
    XAML_RETURN_IF_FAILED(get_parent(&parent));
    if (parent)
    {
        xaml_ptr<xaml_popup_menu_item> multic;
        if (XAML_SUCCEEDED(parent->query(&multic)))
        {
            xaml_ptr<xaml_vector_view<xaml_menu_item>> children;
            XAML_RETURN_IF_FAILED(multic->get_submenu(&children));
            XAML_FOREACH_START(xaml_menu_item, c, children);
            {
                if (auto rc = c.query<xaml_radio_menu_item>())
                {
                    if (rc.get() != static_cast<xaml_control*>(m_outer_this))
                    {
                        xaml_ptr<xaml_string> group;
                        XAML_RETURN_IF_FAILED(rc->get_group(&group));
                        bool equals;
                        XAML_RETURN_IF_FAILED(xaml_string_equals(group, m_group, &equals));
                        if (equals)
                        {
                            xaml_ptr<xaml_gtk3_control> native_control = rc.query<xaml_gtk3_control>();
                            if (native_control)
                            {
                                GtkWidget* native_handle;
                                XAML_RETURN_IF_FAILED(native_control->get_handle(&native_handle));
                                gtk_radio_menu_item_join_group(GTK_RADIO_MENU_ITEM(m_handle), GTK_RADIO_MENU_ITEM(native_handle));
                                break;
                            }
                        }
                    }
                }
            }
            XAML_FOREACH_END();
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_separator_menu_item_internal::draw(xaml_rectangle const&) noexcept
{
    if (!m_handle)
    {
        m_handle = gtk_separator_menu_item_new();
        {
            xaml_ptr<xaml_element_base> parent;
            XAML_RETURN_IF_FAILED(get_parent(&parent));
            xaml_ptr<xaml_gtk3_control> native_parent;
            XAML_RETURN_IF_FAILED(parent->query(&native_parent));
            GtkWidget* parent_handle;
            XAML_RETURN_IF_FAILED(native_parent->get_handle(&parent_handle));
            gtk_menu_shell_append(GTK_MENU_SHELL(parent_handle), m_handle);
        }
    }
    return XAML_S_OK;
}
