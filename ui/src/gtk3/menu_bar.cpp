#include <xaml/ui/menu_bar.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/window.hpp>

using namespace std;

namespace xaml
{
    void menu_bar::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            auto pwnd = get_parent_window().lock();
            auto h = make_shared<native_control>();
            h->handle = gtk_menu_bar_new();
            set_handle(h);
            draw_visible();
            draw_submenu();
            gtk_widget_show_all(get_handle()->handle);
        }
    }

    void menu_bar::draw_submenu()
    {
        for (auto& c : m_children)
        {
            c->__draw({});
            gtk_menu_shell_append(GTK_MENU_SHELL(get_handle()->handle), c->get_handle()->handle);
        }
    }
} // namespace xaml
