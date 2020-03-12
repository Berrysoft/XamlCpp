#import <cocoa/XamlMenuItemDelegate.h>
#include <xaml/ui/controls/menu_item.hpp>
#include <xaml/ui/controls/native_menu_item.hpp>
#include <xaml/ui/menu_bar.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_menu_bar.hpp>

@implementation XamlMenuItemDelegate : XamlDelegate
- (void)onAction
{
    xaml::menu_item* ptr = (xaml::menu_item*)self.classPointer;
    ptr->__on_action();
}
@end

using namespace std;

namespace xaml
{
    void menu_item::__draw(rectangle const& region)
    {
        auto sparent = get_parent().lock();
        NSMenu* hpmenu = nil;
        if (auto pmenu = dynamic_pointer_cast<popup_menu_item>(sparent))
        {
            hpmenu = static_pointer_cast<native_popup_menu_item>(pmenu->get_menu())->menu;
        }
        else if (auto pmenu = dynamic_pointer_cast<menu_bar>(sparent))
        {
            hpmenu = pmenu->get_menu()->handle;
        }
        if (hpmenu)
        {
            auto new_draw = !get_menu();
            draw_append((__bridge void*)hpmenu);
            if (new_draw)
            {
                XamlMenuItemDelegate* delegate = [[XamlMenuItemDelegate alloc] initWithClassPointer:this];
                NSMenuItem* mitem = get_menu()->handle;
                mitem.target = delegate;
                mitem.action = @selector(onAction);
                mitem.title = [NSString stringWithUTF8String:m_text.c_str()];
                auto h = make_shared<native_control>();
                h->delegate = delegate;
                set_handle(h);
                draw_visible();
            }
        }
    }

    void menu_item::draw_visible()
    {
        NSMenuItem* item = (NSMenuItem*)get_menu()->handle;
        item.hidden = !get_is_visible();
    }

    void menu_item::draw_append(void* pmenu)
    {
        if (!get_menu())
        {
            auto m = make_shared<native_menu_item>();
            m->parent = (__bridge NSMenu*)pmenu;
            m->handle = [NSMenuItem new];
            set_menu(m);
            [get_menu()->parent addItem:get_menu()->handle];
        }
    }

    void menu_item::__on_action()
    {
        m_click(*this);
    }

    void popup_menu_item::__draw(rectangle const& region)
    {
        menu_item::__draw(region);
    }

    void popup_menu_item::draw_submenu()
    {
        for (auto& child : m_submenu)
        {
            child->__draw({});
        }
    }

    void popup_menu_item::draw_append(void* pmenu)
    {
        if (!get_menu())
        {
            auto m = make_shared<native_popup_menu_item>();
            m->parent = (__bridge NSMenu*)pmenu;
            m->handle = [NSMenuItem new];
            m->menu = [[NSMenu alloc] initWithTitle:[NSString stringWithUTF8String:get_text().data()]];
            set_menu(m);
            draw_submenu();
            [get_menu()->parent addItem:get_menu()->handle];
            get_menu()->handle.submenu = static_pointer_cast<native_popup_menu_item>(get_menu())->menu;
        }
    }

    void check_menu_item::__draw(rectangle const& region)
    {
        menu_item::__draw(region);
    }

    void check_menu_item::draw_checked()
    {
        get_menu()->handle.state = m_is_checked ? NSControlStateValueOn : NSControlStateValueOff;
    }

    void radio_menu_item::__draw(rectangle const& region)
    {
        menu_item::__draw(region);
    }

    void radio_menu_item::draw_checked()
    {
        get_menu()->handle.state = m_is_checked ? NSControlStateValueOn : NSControlStateValueOff;
    }

    void separator_menu_item::__draw(rectangle const& region)
    {
        menu_item::__draw(region);
    }

    void separator_menu_item::draw_append(void* pmenu)
    {
        if (!get_menu())
        {
            auto m = make_shared<native_menu_item>();
            m->parent = (__bridge NSMenu*)pmenu;
            m->handle = [NSMenuItem separatorItem];
            set_menu(m);
            [get_menu()->parent addItem:get_menu()->handle];
        }
    }
}
