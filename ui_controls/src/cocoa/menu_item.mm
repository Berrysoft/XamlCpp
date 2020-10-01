#import <cocoa/XamlMenuItemDelegate.h>
#include <cocoa/nsstring.hpp>
#include <shared/menu_item.hpp>
#include <xaml/ui/cocoa/menu_bar.h>
#include <xaml/ui/controls/menu_item.h>
#include <xaml/ui/menu_bar.h>

@implementation XamlMenuItemDelegate : XamlDelegate
- (void)onAction
{
    xaml_menu_item_internal* ptr = (xaml_menu_item_internal*)self.classPointer;
    ptr->on_action();
}
@end

using namespace std;

xaml_result xaml_menu_item_internal::draw(xaml_rectangle const&) noexcept
{
    NSMenu* hpmenu = nil;
    xaml_ptr<xaml_element_base> parent;
    XAML_RETURN_IF_FAILED(get_parent(&parent));
    if (parent)
    {
        xaml_ptr<xaml_cocoa_popup_menu_item> native_menu_item;
        if (XAML_SUCCEEDED(parent->query(&native_menu_item)))
        {
            XAML_RETURN_IF_FAILED(native_menu_item->get_handle(&hpmenu));
        }
        else
        {
            xaml_ptr<xaml_cocoa_menu_bar> native_menu_bar;
            if (XAML_SUCCEEDED(parent->query(&native_menu_bar)))
            {
                XAML_RETURN_IF_FAILED(native_menu_bar->get_handle(&hpmenu));
            }
        }
    }
    if (hpmenu)
    {
        auto new_draw = !m_menu;
        XAML_RETURN_IF_FAILED(draw_append(hpmenu));
        if (new_draw)
        {
            XamlMenuItemDelegate* delegate = [[XamlMenuItemDelegate alloc] initWithClassPointer:this];
            NSMenuItem* mitem = m_menu;
            mitem.target = delegate;
            mitem.action = @selector(onAction);
            NSString* title;
            XAML_RETURN_IF_FAILED(get_NSString(m_text, &title));
            mitem.title = title;
            m_delegate = delegate;
            XAML_RETURN_IF_FAILED(draw_visible());
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_menu_item_internal::draw_append(NSMenu* pmenu) noexcept
{
    if (!m_menu)
    {
        m_menu = [NSMenuItem new];
        [pmenu addItem:m_menu];
    }
    return XAML_S_OK;
}

void xaml_menu_item_internal::on_action() noexcept
{
    xaml_ptr<xaml_event_args> args;
    XAML_ASSERT_SUCCEEDED(xaml_event_args_empty(&args));
    XAML_ASSERT_SUCCEEDED(m_click->invoke(m_outer_this, args));
}

xaml_result xaml_popup_menu_item_internal::draw(xaml_rectangle const& region) noexcept
{
    return xaml_menu_item_internal::draw(region);
}

xaml_result xaml_popup_menu_item_internal::draw_submenu() noexcept
{
    XAML_FOREACH_START(xaml_menu_item, cc, m_submenu);
    {
        XAML_RETURN_IF_FAILED(cc->draw({}));
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}

xaml_result xaml_popup_menu_item_internal::draw_append(NSMenu* pmenu) noexcept
{
    if (!m_menu)
    {
        m_menu = [NSMenuItem new];
        NSString* title;
        XAML_RETURN_IF_FAILED(get_NSString(m_text, &title));
        m_menu_handle = [[NSMenu alloc] initWithTitle:title];
        XAML_RETURN_IF_FAILED(draw_submenu());
        [pmenu addItem:m_menu];
        m_menu.submenu = m_menu_handle;
    }
    return XAML_S_OK;
}

xaml_result xaml_check_menu_item_internal::draw(xaml_rectangle const& region) noexcept
{
    return xaml_menu_item_internal::draw(region);
}

xaml_result xaml_check_menu_item_internal::draw_checked() noexcept
{
    m_menu.state = m_is_checked ? NSControlStateValueOn : NSControlStateValueOff;
    return XAML_S_OK;
}

xaml_result xaml_radio_menu_item_internal::draw(xaml_rectangle const& region) noexcept
{
    return xaml_menu_item_internal::draw(region);
}

xaml_result xaml_radio_menu_item_internal::draw_checked() noexcept
{
    m_menu.state = m_is_checked ? NSControlStateValueOn : NSControlStateValueOff;
    return XAML_S_OK;
}

xaml_result xaml_separator_menu_item_internal::draw(xaml_rectangle const& region) noexcept
{
    return xaml_menu_item_internal::draw(region);
}

xaml_result xaml_separator_menu_item_internal::draw_append(NSMenu* pmenu) noexcept
{
    if (!m_menu)
    {
        m_menu = [NSMenuItem separatorItem];
        [pmenu addItem:m_menu];
    }
    return XAML_S_OK;
}
