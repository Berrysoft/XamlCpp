#import <cocoa/XamlWindowDelegate.h>
#include <shared/atomic_guard.hpp>
#include <xaml/ui/application.hpp>
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
        [__get_window() close];
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
            __set_window(window);
            set_handle(window.contentView);
            application::current()->window_added(static_pointer_cast<xaml::window>(shared_from_this()));
        }
        NSWindow* window = __get_window();
        {
            atomic_guard guard{ m_resizing };
            if (!guard.exchange(true))
            {
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
        draw_title();
        if (get_child())
        {
            draw_child();
        }
        draw_resizable();
    }

    void window::__parent_redraw()
    {
        if (__get_window())
            __draw({});
    }

    void window::draw_title()
    {
        NSWindow* window = __get_window();
        NSString* nstitle = [NSString stringWithUTF8String:m_title.data()];
        [window setTitle:nstitle];
    }

    void window::draw_child()
    {
        get_child()->__draw(get_client_region());
    }

    void window::draw_resizable()
    {
        NSWindow* window = __get_window();
        if (m_resizable)
            window.styleMask |= NSWindowStyleMaskResizable;
        else
            window.styleMask &= ~NSWindowStyleMaskResizable;
    }

    void window::show()
    {
        __draw({});
        NSWindow* window = __get_window();
        [window makeKeyAndOrderFront:nil];
    }

    void window::close()
    {
        [__get_window() performClose:nil];
    }

    void window::hide()
    {
        [__get_window() setIsVisible:NO];
    }

    rectangle window::get_client_region() const
    {
        NSWindow* window = __get_window();
        NSRect frame = [[window contentView] frame];
        return from_native(frame);
    }

    void window::__on_did_resize()
    {
        NSWindow* window = __get_window();
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
