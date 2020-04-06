#include <xaml/ui/application.hpp>
#include <xaml/ui/menu_bar.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_menu_bar.hpp>
#include <xaml/ui/native_window.hpp>
#include <xaml/ui/window.hpp>

using namespace std;

namespace xaml
{
    void menu_bar::__draw(rectangle const& region)
    {
        if (!get_menu())
        {
            auto pwnd = get_parent_window().lock();
            auto m = make_shared<native_menu_bar>();
            m->handle = pwnd->get_window()->menu_bar;
            set_menu(m);
            draw_submenu();
        }
    }

    void menu_bar::draw_visible()
    {
    }

    void menu_bar::draw_submenu()
    {
        for (auto& c : get_children())
        {
            c->__draw({});
        }
    }
}
