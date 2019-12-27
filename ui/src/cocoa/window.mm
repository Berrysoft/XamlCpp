#import <internal/cocoa/XamlWindowDelegate.h>
#include <internal/cocoa/drawing.hpp>
#include <xaml/ui/application.hpp>
#include <xaml/ui/window.hpp>

#ifdef XAML_UI_USE_GNUSTEP
typedef enum NSWindowStyleMask : NSUInteger
{
    NSWindowStyleMaskBorderless = 0,
    NSWindowStyleMaskTitled = 1 << 0,
    NSWindowStyleMaskClosable = 1 << 1,
    NSWindowStyleMaskMiniaturizable = 1 << 2,
    NSWindowStyleMaskResizable = 1 << 3
} NSWindowStyleMask;
#endif // XAML_UI_USE_GNUSTEP

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
        add_title_changed([this](window const&, string_view_t) { if (get_handle()) draw_title(); });
        add_location_changed([this](window const&, point) { if (get_handle() && !resizing) __draw({}); });
        add_size_changed([this](control const&, size) { if (get_handle() && !resizing) __draw({}); });
        add_resizable_changed([this](control const&, bool) { if(get_handle()) draw_resizable(); });
    }

    window::~window()
    {
        [(NSWindow*)get_handle() close];
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
            __set_delegate(delegate);
            window.delegate = delegate;
            set_handle(window);
            application::current()->wnd_num++;
        }
        NSWindow* window = (NSWindow*)get_handle();
        if (!resizing)
        {
            resizing = true;
            CGFloat fw = (CGFloat)get_width();
            CGFloat fh = (CGFloat)get_height();
            NSRect frame = [window frame];
            frame.size = { fw, fh };
            frame.origin.x = (CGFloat)get_x();
            NSScreen* screen = window.screen;
            NSRect screen_frame = screen.visibleFrame;
            frame.origin.y = screen_frame.size.height - fh - (CGFloat)get_y();
            [window setFrame:frame display:YES];
            resizing = false;
        }
        draw_title();
        if (get_child())
        {
            draw_child();
        }
        draw_resizable();
    }

    void window::draw_title()
    {
        NSWindow* window = (NSWindow*)get_handle();
        NSString* nstitle = [NSString stringWithUTF8String:m_title.data()];
        [window setTitle:nstitle];
    }

    void window::draw_child()
    {
        get_child()->__draw(get_client_region());
    }

    void window::draw_resizable()
    {
#ifndef XAML_UI_USE_GNUSTEP
        NSWindow* window = (NSWindow*)get_handle();
        if (m_resizable)
            window.styleMask |= NSWindowStyleMaskResizable;
        else
            window.styleMask &= ~NSWindowStyleMaskResizable;
#endif // !XAML_UI_USE_GNUSTEP
    }

    void window::show()
    {
        __draw({});
        NSWindow* window = (NSWindow*)get_handle();
        [window makeKeyAndOrderFront:nil];
    }

    rectangle window::get_client_region() const
    {
        NSWindow* window = (NSWindow*)get_handle();
        NSRect frame = [[window contentView] frame];
        return get_rect(frame);
    }

    void window::__on_did_resize()
    {
        NSWindow* window = (NSWindow*)get_handle();
        NSRect frame = window.frame;
        if (!resizing)
        {
            resizing = true;
            set_size(xaml::get_size(frame.size));
            __draw({});
            resizing = false;
        }
    }

    bool window::__on_should_close()
    {
        bool handled;
        m_closing(*this, handled);
        return !handled;
    }
}
