#include <wil/result_macros.h>
#include <xaml/ui/menu_bar.h>
#include <xaml/ui/window.h>

using namespace std;

//namespace xaml
//{
//    void menu_bar::__draw(rectangle const& region)
//    {
//        set_handle(get_parent_window().lock()->get_handle());
//        if (!get_menu())
//        {
//            auto m = make_shared<native_menu_bar>();
//            m->handle.reset(CreateMenu());
//            set_menu(m);
//            draw_visible();
//        }
//        draw_submenu();
//        THROW_IF_WIN32_BOOL_FALSE(DrawMenuBar(get_handle()->handle));
//    }
//
//    void menu_bar::draw_submenu()
//    {
//        for (auto& child : get_children())
//        {
//            child->__draw({});
//        }
//    }
//
//    void menu_bar::draw_visible()
//    {
//        if (get_is_visible())
//            THROW_IF_WIN32_BOOL_FALSE(SetMenu(get_handle()->handle, get_menu()->handle.get()));
//        else
//        {
//            if (GetMenu(get_handle()->handle) == get_menu()->handle.get())
//                THROW_IF_WIN32_BOOL_FALSE(SetMenu(get_handle()->handle, nullptr));
//        }
//    }
//} // namespace xaml
