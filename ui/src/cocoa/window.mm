#import <cocoa/XamlWindowDelegate.h>
#include <cocoa/nsstring.hpp>
#include <shared/atomic_guard.hpp>
#include <shared/window.hpp>
#include <xaml/ui/application.h>
#include <xaml/ui/native_drawing.hpp>
#include <xaml/ui/window.h>

@implementation XamlWindowDelegate : XamlDelegate
- (void)windowDidResize:(NSNotification*)notification
{
    xaml_window_internal* window = (xaml_window_internal*)self.classPointer;
    window->on_did_resize();
}

- (void)windowDidMove:(NSNotification*)notification
{
    xaml_window_internal* window = (xaml_window_internal*)self.classPointer;
    window->on_did_move();
}

- (BOOL)windowShouldClose:(NSWindow*)sender
{
    xaml_window_internal* window = (xaml_window_internal*)self.classPointer;
    BOOL close = window->on_should_close();
    return close;
}

- (void)onQuitAction
{
    xaml_ptr<xaml_application> app;
    XAML_ASSERT_SUCCEEDED(xaml_application_current(&app));
    XAML_ASSERT_SUCCEEDED(app->quit(0));
}
@end

using namespace std;

xaml_window_internal::~xaml_window_internal()
{
    [m_window_handle close];
}

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

static void add_system_menu_items(NSMenu* menu, XamlWindowDelegate* delegate)
{
    [menu addItem:create_menu_item(@"Quit", delegate, @selector(onQuitAction), NSEventModifierFlagCommand, @"q")];
}

xaml_result xaml_window_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        NSRect frame = NSMakeRect(0, 0, m_size.width, m_size.height);
        NSWindow* window = [[NSWindow alloc] initWithContentRect:frame
                                                       styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable
                                                         backing:NSBackingStoreBuffered
                                                           defer:NO];
        XamlWindowDelegate* delegate = [[XamlWindowDelegate alloc] initWithClassPointer:this];
        window.delegate = delegate;
        m_window_handle = window;
        m_menu_bar_handle = [NSMenu new];
        m_default_menu_item_handle = [NSMenuItem new];
        [m_menu_bar_handle addItem:m_default_menu_item_handle];
        m_default_menu_item_menu_handle = [NSMenu new];
        m_default_menu_item_handle.submenu = m_default_menu_item_menu_handle;
        add_system_menu_items(m_default_menu_item_menu_handle, delegate);
        m_handle = window.contentView;
        xaml_ptr<xaml_application> app;
        XAML_RETURN_IF_FAILED(xaml_application_current(&app));
        XAML_RETURN_IF_FAILED(app->window_added(static_cast<xaml_window*>(m_outer_this)));
        XAML_RETURN_IF_FAILED(draw_title());
        XAML_RETURN_IF_FAILED(draw_resizable());
    }
    XAML_RETURN_IF_FAILED(draw_size());
    XAML_RETURN_IF_FAILED(draw_child());
    XAML_RETURN_IF_FAILED(draw_menu_bar());
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_visible() noexcept
{
    [m_window_handle setIsVisible:m_is_visible];
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_size() noexcept
{
    xaml_atomic_guard guard{ m_resizing };
    if (!guard.test_and_set())
    {
        CGFloat fw = m_size.width;
        CGFloat fh = m_size.height;
        NSRect frame = [m_window_handle frame];
        frame.size = { fw, fh };
        frame.origin.x = m_location.x;
        NSScreen* screen = m_window_handle.screen;
        NSRect screen_frame = screen.frame;
        frame.origin.y = screen_frame.size.height - fh - m_location.y;
        [m_window_handle setFrame:frame display:YES];
    }
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_title() noexcept
{
    NSString* title;
    XAML_RETURN_IF_FAILED(get_NSString(m_title, &title));
    m_window_handle.title = title;
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_child() noexcept
{
    if (m_child)
    {
        xaml_rectangle region;
        XAML_RETURN_IF_FAILED(get_client_region(&region));
        return m_child->draw(region);
    }
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_resizable() noexcept
{
    if (m_is_resizable)
        m_window_handle.styleMask |= NSWindowStyleMaskResizable;
    else
        m_window_handle.styleMask &= ~NSWindowStyleMaskResizable;
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_menu_bar() noexcept
{
    if (m_menu_bar)
    {
        XAML_RETURN_IF_FAILED(m_menu_bar->set_parent(static_cast<xaml_control*>(m_outer_this)));
        XAML_RETURN_IF_FAILED(m_menu_bar->draw({}));
    }
    [NSApp setMainMenu:m_menu_bar_handle];
    return XAML_S_OK;
}

xaml_result xaml_window_internal::show() noexcept
{
    XAML_RETURN_IF_FAILED(draw({}));
    XAML_RETURN_IF_FAILED(set_is_visible(true));
    [m_window_handle makeKeyAndOrderFront:nil];
    return XAML_S_OK;
}

xaml_result xaml_window_internal::close() noexcept
{
    [m_window_handle performClose:nil];
    return XAML_S_OK;
}

xaml_result xaml_window_internal::hide() noexcept
{
    return set_is_visible(false);
}

xaml_result xaml_window_internal::get_client_region(xaml_rectangle* pregion) noexcept
{
    NSRect const& frame = m_window_handle.contentView.frame;
    *pregion = xaml_from_native(frame);
    return XAML_S_OK;
}

void xaml_window_internal::on_did_resize() noexcept
{
    NSRect const& frame = m_window_handle.frame;
    xaml_atomic_guard guard{ m_resizing };
    if (!guard.test_and_set())
    {
        XAML_ASSERT_SUCCEEDED(set_size(xaml_from_native(frame.size)));
        XAML_ASSERT_SUCCEEDED(draw({}));
    }
}

void xaml_window_internal::on_did_move() noexcept
{
    NSRect const& frame = m_window_handle.frame;
    xaml_atomic_guard guard{ m_resizing };
    if (!guard.test_and_set())
    {
        XAML_ASSERT_SUCCEEDED(set_location(xaml_from_native(frame.origin)));
        XAML_ASSERT_SUCCEEDED(draw({}));
    }
}

bool xaml_window_internal::on_should_close() noexcept
{
    xaml_ptr<xaml_object> handled;
    XAML_ASSERT_SUCCEEDED(xaml_box_value(false, &handled));
    XAML_ASSERT_SUCCEEDED(on_closing(m_outer_this, handled));
    bool value;
    XAML_ASSERT_SUCCEEDED(xaml_unbox_value(handled, &value));
    if (!value)
    {
        xaml_ptr<xaml_application> app;
        XAML_ASSERT_SUCCEEDED(xaml_application_current(&app));
        XAML_ASSERT_SUCCEEDED(app->window_removed(static_cast<xaml_window*>(m_outer_this)));
    }
    return !value;
}

xaml_result xaml_window_internal::get_dpi(double* pvalue) noexcept
{
    *pvalue = 72.0;
    return XAML_S_OK;
}
