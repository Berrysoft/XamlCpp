#import <cocoa/XamlSystemMenuItemDelegate.h>
#include <xaml/ui/application.hpp>
#include <xaml/ui/menu_bar.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_menu_bar.hpp>
#include <xaml/ui/window.hpp>

@implementation XamlSystemMenuItemDelegate : XamlDelegate
- (void)onQuitAction
{
    xaml::application::current()->quit(0);
}
@end

using namespace std;

namespace xaml
{
    static NSMenuItem* create_menu_item(NSString* text, id delegate, SEL action, NSEventModifierFlags modifier = 0, NSString* key = @"")
    {
        NSMenuItem* mitem = [NSMenuItem new];
        mitem.target = delegate;
        mitem.action = action;
        mitem.title = text;
        mitem.keyEquivalent = key;
        mitem.keyEquivalentModifierMask = modifier;
        return mitem;
	}

    static void add_system_menu_items(NSMenu* menu, XamlSystemMenuItemDelegate* delegate)
    {
        [menu addItem:create_menu_item(@"Quit", delegate, @selector(onQuitAction), NSEventModifierFlagCommand, @"q")];
	}

    void menu_bar::__draw(rectangle const& region)
    {
        if (!get_menu())
        {
            auto pwnd = get_parent_window().lock();
            auto m = make_shared<native_menu_bar>();
            m->handle = [NSMenu new];
            m->default_item = [NSMenuItem new];
            m->default_item_menu = [NSMenu new];
            m->default_item.submenu = m->default_item_menu;
            XamlSystemMenuItemDelegate* delegate = [[XamlSystemMenuItemDelegate alloc] initWithClassPointer:this];
            auto h = make_shared<native_control>();
            h->delegate = delegate;
            set_handle(h);
            add_system_menu_items(m->default_item_menu, delegate);
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
