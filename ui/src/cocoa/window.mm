#import <cocoa/XamlWindowDelegate.h>
#include <shared/atomic_guard.hpp>
#include <xaml/ui/application.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_drawing.hpp>
#include <xaml/ui/native_menu_bar.hpp>
#include <xaml/ui/native_window.hpp>
#include <xaml/ui/window.hpp>

@implementation XamlWindowDelegate : XamlDelegate
- (void)windowDidResize:(NSNotification*)notification
{
    xaml::window* window = (xaml::window*)self->classPointer;
    window->__on_did_resize();
}

- (BOOL)windowShouldClose:(NSWindow*)sender
{
    xaml::window* window = (xaml::window*)self->classPointer;
    BOOL close = window->__on_should_close() ? YES : NO;
    if (close)
    {
        xaml::application::current()->window_removed(std::static_pointer_cast<xaml::window>(window->shared_from_this()));
    }
    return close;
}
@end

using namespace std;

namespace xaml
{
    window::~window()
    {
        NSWindow* window = get_window()->window;
        [window close];
    }

    void window::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            NSRect frame = NSMakeRect(0, 0, get_width(), get_height());
            NSWindow* window = [[NSWindow alloc] initWithContentRect:frame
                                                           styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable
                                                             backing:NSBackingStoreBuffered
                                                               defer:NO];
            XamlWindowDelegate* delegate = [[XamlWindowDelegate alloc] initWithClassPointer:this];
            window.delegate = delegate;
            auto h = make_shared<native_control>();
            auto w = make_shared<native_window>();
            w->window = window;
            h->handle = window.contentView;
            set_window(w);
            set_handle(h);
            application::current()->window_added(static_pointer_cast<xaml::window>(shared_from_this()));
            draw_title();
            draw_resizable();
        }
        draw_size();
        draw_child();
        draw_menu_bar();
    }

    void window::__parent_redraw()
    {
        if (get_window())
            __draw({});
    }

    void window::draw_size()
    {
        atomic_guard guard{ m_resizing };
        if (!guard.exchange(true))
        {
            NSWindow* window = get_window()->window;
            CGFloat fw = (CGFloat)get_width();
            CGFloat fh = (CGFloat)get_height();
            NSRect frame = [window frame];
            frame.size = { fw, fh };
            frame.origin.x = (CGFloat)get_x();
            NSScreen* screen = window.screen;
            NSRect screen_frame = screen.frame;
            frame.origin.y = screen_frame.size.height - fh - (CGFloat)get_y();
            [window setFrame:frame display:YES];
        }
    }

    void window::draw_title()
    {
        NSWindow* window = get_window()->window;
        NSString* nstitle = [NSString stringWithUTF8String:m_title.data()];
        [window setTitle:nstitle];
    }

    void window::draw_child()
    {
        if (get_child())
            get_child()->__draw(get_client_region());
    }

    void window::draw_resizable()
    {
        NSWindow* window = get_window()->window;
        if (m_resizable)
            window.styleMask |= NSWindowStyleMaskResizable;
        else
            window.styleMask &= ~NSWindowStyleMaskResizable;
    }

    void window::draw_menu_bar()
    {
        if (get_menu_bar())
        {
            get_menu_bar()->set_parent_window(static_pointer_cast<window>(shared_from_this()));
            get_menu_bar()->__draw({});
            [NSApp setMainMenu:get_menu_bar()->get_menu()->handle];
        }
    }

    void window::show()
    {
        __draw({});
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
        NSWindow* window = get_window()->window;
        [window setIsVisible:NO];
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
        {
            atomic_guard guard{ m_resizing };
            if (!guard.exchange(true))
            {
                set_size(from_native(frame.size));
                __draw({});
            }
        }
    }

    bool window::__on_should_close()
    {
        bool handled;
        m_closing(*this, handled);
        return !handled;
    }

    double window::get_dpi() const
    {
        return 72.0;
    }
}
