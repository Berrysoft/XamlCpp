#import <cocoa/XamlWindowDelegate.h>
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
    // if (close)
    // {
    //     xaml::application::current()->window_removed(window->shared_from_this<xaml::window>());
    // }
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
    char const* data = nullptr;
    if (m_title)
    {
        XAML_RETURN_IF_FAILED(m_title->get_data(&data));
    }
    m_window_handle.title = [NSString stringWithUTF8String:data];
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

void window::draw_menu_bar()
{
    if (get_menu_bar())
    {
        get_menu_bar()->set_parent_window(shared_from_this<window>());
        get_menu_bar()->__draw({});
    }
    [NSApp setMainMenu:get_window()->menu_bar];
}

void window::show()
{
    __draw({});
    set_is_visible(true);
    NSWindow* window = get_window()->window;
    [window makeKeyAndOrderFront:nil];
}

void window::close()
{
    NSWindow* window = get_window()->window;
    [window performClose:nil];
}

void window::hide()
{
    set_is_visible(false);
}

rectangle window::get_client_region() const
{
    NSWindow* window = get_window()->window;
    NSRect frame = [[window contentView] frame];
    return from_native(frame);
}

void window::__on_did_resize()
{
    NSWindow* window = get_window()->window;
    NSRect frame = window.frame;
    atomic_guard guard{ m_resizing };
    if (!guard.test_and_set())
    {
        set_size(from_native(frame.size));
        __draw({});
    }
}

void window::__on_did_move()
{
    NSWindow* window = get_window()->window;
    NSRect frame = window.frame;
    atomic_guard guard{ m_resizing };
    if (!guard.test_and_set())
    {
        set_location(from_native(frame.origin));
        __draw({});
    }
}

bool window::__on_should_close()
{
    auto handled = box_value(false);
    m_closing(shared_from_this<window>(), handled);
    return !*handled;
}

double window::get_dpi() const
{
    return 72.0;
}
