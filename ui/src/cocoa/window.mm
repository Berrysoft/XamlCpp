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
    return window->__on_should_close() ? YES : NO;
}
@end

using namespace std;

namespace xaml
{
    window::window() : container(), m_resizable(true)
    {
        add_title_changed([this](window const&, string_view_t) { if (__get_window()) draw_title(); });
        add_location_changed([this](window const&, point) { if (__get_window() && !m_resizing) __draw({}); });
        add_size_changed([this](control const&, size) { if (__get_window() && !m_resizing) __draw({}); });
        add_resizable_changed([this](control const&, bool) { if(__get_window()) draw_resizable(); });
    }

    window::~window()
    {
        [__get_window() close];
    }

    void window::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            NSRect frame = NSMakeRect(0, 0, get_width(), get_height());
            NSWindow* window = [[NSWindow alloc]
                initWithContentRect:frame
                          styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable
                            backing:NSBackingStoreBuffered
                              defer:NO];
            XamlWindowDelegate* delegate = [[XamlWindowDelegate alloc] initWithClassPointer:this];
            window.delegate = delegate;
            __set_window(window);
            set_handle(window.contentView);
            application::current()->wnd_num++;
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
}
