#include <xaml/ui/controls/menu_item.hpp>
#include <xaml/ui/menu_bar.hpp>

using namespace std;

namespace xaml
{
    menu_item::menu_item() : control() {}

    menu_item::~menu_item() {}

    popup_menu_item::popup_menu_item() : menu_item() {}

    popup_menu_item::~popup_menu_item() {}

    void popup_menu_item::add_submenu(shared_ptr<menu_item> const& child)
    {
        if (child)
        {
            auto it = find(m_submenu.begin(), m_submenu.end(), child);
            if (it == m_submenu.end())
            {
                m_submenu.push_back(child);
                child->set_parent(static_pointer_cast<control>(shared_from_this()));
                __parent_redraw();
            }
        }
    }

    void popup_menu_item::remove_submenu(shared_ptr<menu_item> const& child)
    {
        auto it = find(m_submenu.begin(), m_submenu.end(), child);
        if (it != m_submenu.end())
        {
            child->set_parent({});
            m_submenu.erase(it);
            __parent_redraw();
        }
    }

    check_menu_item::check_menu_item() : menu_item()
    {
        add_is_checked_changed([this](shared_ptr<check_menu_item>, bool) { if(get_handle()) draw_checked(); });
    }

    check_menu_item::~check_menu_item() {}

    radio_menu_item::radio_menu_item() : menu_item()
    {
        add_is_checked_changed([this](shared_ptr<radio_menu_item>, bool) {
            if (get_handle())
            {
                draw_checked();
                draw_group();
            }
        });
    }

    radio_menu_item::~radio_menu_item() {}

#ifndef XAML_UI_GTK3
    void radio_menu_item::draw_group()
    {
        if (auto sparent = get_parent().lock())
        {
            if (get_is_checked())
            {
                if (auto multic = dynamic_pointer_cast<menu_bar>(sparent))
                {
                    for (auto& c : multic->get_children())
                    {
                        if (auto rc = dynamic_pointer_cast<radio_menu_item>(c))
                        {
                            if (c != shared_from_this() && rc->get_group() == get_group())
                            {
                                rc->set_is_checked(false);
                            }
                        }
                    }
                }
            }
        }
    }
#endif // !XAML_UI_GTK3

    separator_menu_item::separator_menu_item() : menu_item()
    {
    }

    separator_menu_item::~separator_menu_item() {}
} // namespace xaml
