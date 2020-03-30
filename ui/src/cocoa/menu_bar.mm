#include <xaml/ui/menu_bar.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_menu_bar.hpp>
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
            m->handle = [NSMenu new];
            set_menu(m);
            draw_visible();
            draw_submenu();
        }
    }

    void menu_bar::draw_visible()
    {
        if (get_is_visible())
            [NSApp setMainMenu:get_menu()->handle];
        else
        {
            if (NSApp.mainMenu == get_menu()->handle)
                [NSApp setMainMenu:nil];
        }
    }

    void menu_bar::draw_submenu()
    {
        for (auto& c : get_children())
        {
            c->__draw({});
        }
    }
}
