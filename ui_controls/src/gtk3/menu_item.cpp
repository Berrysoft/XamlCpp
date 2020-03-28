#include <xaml/ui/controls/menu_item.hpp>
#include <xaml/ui/native_control.hpp>

using namespace std;

namespace xaml
{
    void menu_item::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            auto sparent = get_parent().lock();
            auto h = make_shared<native_control>();
            h->handle = gtk_menu_item_new_with_label(m_text.c_str());
            set_handle(h);
            g_signal_connect(G_OBJECT(get_handle()->handle), "activate", G_CALLBACK(menu_item::on_activate), this);
            draw_visible();
        }
    }

    void menu_item::on_activate(void* m, void* data)
    {
        menu_item* self = (menu_item*)data;
        self->m_click(static_pointer_cast<menu_item>(self->shared_from_this()));
    }

    void popup_menu_item::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            menu_item::__draw(region);
            draw_submenu();
        }
    }

    void popup_menu_item::draw_submenu()
    {
        GtkWidget* subm = gtk_menu_new();
        for (auto& m : m_submenu)
        {
            m->__draw({});
            gtk_menu_shell_append(GTK_MENU_SHELL(subm), m->get_handle()->handle);
        }
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(get_handle()->handle), subm);
    }

    void check_menu_item::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            auto sparent = get_parent().lock();
            auto h = make_shared<native_control>();
            h->handle = gtk_check_menu_item_new_with_label(get_text().data());
            set_handle(h);
            g_signal_connect(G_OBJECT(get_handle()->handle), "activate", G_CALLBACK(menu_item::on_activate), this);
            gtk_menu_shell_append(GTK_MENU_SHELL(sparent->get_handle()->handle), get_handle()->handle);
            draw_checked();
        }
    }

    void check_menu_item::draw_checked()
    {
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(get_handle()->handle), m_is_checked ? TRUE : FALSE);
    }

    void radio_menu_item::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            auto sparent = get_parent().lock();
            auto h = make_shared<native_control>();
            h->handle = gtk_radio_menu_item_new_with_label(nullptr, get_text().data());
            set_handle(h);
            g_signal_connect(G_OBJECT(get_handle()->handle), "activate", G_CALLBACK(menu_item::on_activate), this);
            gtk_menu_shell_append(GTK_MENU_SHELL(sparent->get_handle()->handle), get_handle()->handle);
            draw_checked();
            draw_group();
        }
    }

    void radio_menu_item::draw_checked()
    {
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(get_handle()->handle), m_is_checked ? TRUE : FALSE);
    }

    void radio_menu_item::draw_group()
    {
        if (auto sparent = get_parent().lock())
        {
            if (auto multic = sparent->query<popup_menu_item>())
            {
                for (auto& c : multic->get_submenu())
                {
                    if (c)
                    {
                        if (auto rc = c->query<radio_menu_item>())
                        {
                            if (c != shared_from_this() && c->get_handle() && rc->get_group() == get_group())
                            {
                                gtk_radio_menu_item_join_group(GTK_RADIO_MENU_ITEM(get_handle()->handle), GTK_RADIO_MENU_ITEM(c->get_handle()->handle));
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    void separator_menu_item::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            auto sparent = get_parent().lock();
            auto h = make_shared<native_control>();
            h->handle = gtk_separator_menu_item_new();
            set_handle(h);
            gtk_menu_shell_append(GTK_MENU_SHELL(sparent->get_handle()->handle), get_handle()->handle);
        }
    }
} // namespace xaml
