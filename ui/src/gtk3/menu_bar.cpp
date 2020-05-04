#include <shared/menu_bar.hpp>
#include <xaml/ui/menu_bar.h>
#include <xaml/ui/window.h>

using namespace std;

xaml_result xaml_menu_bar_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        m_handle = gtk_menu_bar_new();
        m_menu = GTK_MENU_BAR(m_handle);
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_submenu());
        gtk_widget_show_all(m_handle);
    }
    return XAML_S_OK;
}

xaml_result xaml_menu_bar_internal::draw_submenu() noexcept
{
    XAML_FOREACH_START(child, m_children);
    {
        xaml_ptr<xaml_control> cc;
        XAML_RETURN_IF_FAILED(child->query(&cc));
        XAML_RETURN_IF_FAILED(cc->draw({}));
        xaml_ptr<xaml_gtk3_control> native_control = cc.query<xaml_gtk3_control>();
        if (native_control)
        {
            GtkWidget* native_handle;
            XAML_RETURN_IF_FAILED(native_control->get_handle(&native_handle));
            gtk_menu_shell_append(GTK_MENU_SHELL(m_handle), native_handle);
        }
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}
