#include <wil/result_macros.h>
#include <xaml/ui/menu_bar.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_menu_bar.hpp>
#include <xaml/ui/window.hpp>

using namespace std;

namespace xaml
{
    void menu_bar::__draw(rectangle const& region)
    {
        set_handle(get_parent_window().lock()->get_handle());
        if (!get_menu())
        {
            auto m = make_shared<native_menu_bar>();
            m->handle.reset(CreateMenu());
            set_menu(m);
            THROW_IF_WIN32_BOOL_FALSE(SetMenu(get_handle()->handle, get_menu()->handle.get()));
        }
        draw_submenu();
        THROW_IF_WIN32_BOOL_FALSE(DrawMenuBar(get_handle()->handle));
    }

    void menu_bar::draw_submenu()
    {
        for (auto& child : m_children)
        {
            child->__draw({});
        }
    }
} // namespace xaml
