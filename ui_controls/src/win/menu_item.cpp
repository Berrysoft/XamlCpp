#include <wil/result_macros.h>
#include <xaml/ui/controls/menu_item.hpp>
#include <xaml/ui/controls/native_menu_item.hpp>
#include <xaml/ui/menu_bar.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_menu_bar.hpp>

using namespace std;

namespace xaml
{
    optional<intptr_t> menu_item::__wnd_proc(window_message const& msg)
    {
        switch (msg.Msg)
        {
        case WM_COMMAND:
        {
            if (get_menu() && LOWORD(msg.wParam) == get_menu()->handle)
            {
                m_click(static_pointer_cast<menu_item>(shared_from_this()));
            }
            break;
        }
        }
        return nullopt;
    }

    static UINT menu_id{ 0 };

    void menu_item::__draw_impl(uint32_t flags)
    {
        auto sparent = get_parent().lock();
        HMENU hpmenu = nullptr;
        if (sparent)
        {
            if (auto ppmenu = sparent->query<popup_menu_item>())
            {
                hpmenu = static_pointer_cast<native_popup_menu_item>(ppmenu->get_menu())->menu.get();
            }
            else if (auto pmenu = sparent->query<menu_bar>())
            {
                hpmenu = pmenu->get_menu()->handle.get();
            }
        }
        if (hpmenu)
        {
            set_handle(sparent->get_handle());
            if (!get_menu())
            {
                draw_append(hpmenu, flags);
            }
            else if (hpmenu != get_menu()->parent)
            {
                THROW_IF_WIN32_BOOL_FALSE(DeleteMenu(get_menu()->parent, get_menu()->handle, MF_BYCOMMAND));
                get_menu()->parent = hpmenu;
                draw_append(hpmenu, flags);
            }
        }
    }

    void menu_item::__draw(rectangle const& region)
    {
        __draw_impl(MF_STRING);
    }

    void menu_item::draw_append(void* pmenu, uint32_t flags)
    {
        if (!get_menu())
        {
            auto m = make_shared<native_menu_item>();
            m->parent = (HMENU)pmenu;
            m->handle = menu_id++;
            set_menu(m);
        }
        THROW_IF_WIN32_BOOL_FALSE(InsertMenu(get_menu()->parent, get_menu()->handle, flags, get_menu()->handle, m_text.c_str()));
    }

    optional<intptr_t> popup_menu_item::__wnd_proc(window_message const& msg)
    {
        optional<std::intptr_t> result = nullopt;
        for (auto& c : m_submenu)
        {
            auto r = c->__wnd_proc(msg);
            if (r) result = r;
        }
        return result;
    }

    void popup_menu_item::__draw(rectangle const& region)
    {
        __draw_impl(MF_STRING | MF_POPUP);
    }

    void popup_menu_item::draw_submenu()
    {
        for (auto& child : m_submenu)
        {
            child->__draw({});
        }
    }

    void popup_menu_item::draw_append(void* pmenu, uint32_t flags)
    {
        if (!get_menu())
        {
            auto m = make_shared<native_popup_menu_item>();
            m->parent = (HMENU)pmenu;
            m->handle = menu_id++;
            m->menu.reset(CreateMenu());
            set_menu(m);
            draw_submenu();
        }
        auto pm = static_pointer_cast<native_popup_menu_item>(get_menu());
        THROW_IF_WIN32_BOOL_FALSE(InsertMenu(get_menu()->parent, pm->handle, flags, (UINT_PTR)pm->menu.get(), get_text().data()));
    }

    void check_menu_item::__draw(rectangle const& region)
    {
        __draw_impl(MF_STRING | MF_UNCHECKED);
    }

    void check_menu_item::draw_checked()
    {
        CheckMenuItem(get_menu()->parent, get_menu()->handle, MF_BYCOMMAND | (m_is_checked ? MF_CHECKED : MF_UNCHECKED));
    }

    void radio_menu_item::__draw(rectangle const& region)
    {
        __draw_impl(MF_STRING | MF_UNCHECKED);
        MENUITEMINFO info;
        info.cbSize = sizeof(MENUITEMINFO);
        THROW_IF_WIN32_BOOL_FALSE(GetMenuItemInfo(get_menu()->parent, get_menu()->handle, FALSE, &info));
        info.fType |= MFT_RADIOCHECK;
        THROW_IF_WIN32_BOOL_FALSE(SetMenuItemInfo(get_menu()->parent, get_menu()->handle, FALSE, &info));
    }

    void radio_menu_item::draw_checked()
    {
        CheckMenuItem(get_menu()->parent, get_menu()->handle, MF_BYCOMMAND | (m_is_checked ? MF_CHECKED : MF_UNCHECKED));
    }

    void separator_menu_item::__draw(rectangle const& region)
    {
        __draw_impl(MF_SEPARATOR);
    }
} // namespace xaml
