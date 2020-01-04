#include <cocoa/drawing.hpp>
#import <internal/cocoa/XamlButtonDelegate.h>
#include <xaml/ui/button.hpp>

@implementation XamlButtonDelegate : XamlDelegate
- (NSButton*)newButton
{
    NSButton* button = [NSButton new];
    button.target = self;
    button.action = @selector(onAction);
    return button;
}

- (void)onAction
{
    xaml::button_base* ptr = (xaml::button_base*)self.classPointer;
    ptr->__on_action();
}
@end

using namespace std;

namespace xaml
{
    button_base::button_base() : common_control()
    {
        add_text_changed([this](button_base const&, string_view_t) {
            if (get_handle())
            {
                draw_text();
                __parent_redraw();
            }
        });
        add_size_changed([this](control const&, size) {
            if (get_handle())
            {
                draw_size();
                __parent_redraw();
            }
        });
        add_is_default_changed([this](button_base const&, bool) { if (get_handle()) draw_default(); });
    }

    button_base::~button_base()
    {
    }

    void button_base::__draw(const rectangle& region)
    {
        if (!get_handle())
        {
            XamlButtonDelegate* delegate = [[XamlButtonDelegate alloc] initWithClassPointer:this];
            __set_delegate(delegate);
            set_handle([delegate newButton]);
        }
        rectangle real = region - get_margin();
        NSButton* button = (NSButton*)get_handle();
        [button setBezelStyle:NSBezelStyleRounded];
        NSView* view = get_parent()->get_handle();
        NSRect parent_frame = [view frame];
        NSRect frame = button.frame;
        frame.origin = { real.x, parent_frame.size.height - real.height - real.y };
        frame.size = { real.width, real.height };
        button.frame = frame;
        __set_size_noevent({ real.width, real.height });
        draw_text();
        draw_default();
    }

    void button_base::draw_text()
    {
        NSButton* button = (NSButton*)get_handle();
        NSString* ns_title = [NSString stringWithUTF8String:m_text.c_str()];
        button.title = ns_title;
    }

    void button_base::draw_size()
    {
        NSButton* button = (NSButton*)get_handle();
        NSRect frame = button.frame;
        frame.size = get_NSSize(get_size());
        button.frame = frame;
    }

    void button_base::draw_default()
    {
        NSButton* button = (NSButton*)get_handle();
        if (m_is_default)
        {
            button.keyEquivalent = @"\r";
            [button highlight:YES];
        }
        else
        {
            button.keyEquivalent = @"";
            [button highlight:NO];
        }
    }

    void button_base::__on_action()
    {
        m_click(*this);
    }

    void button_base::__size_to_fit()
    {
        NSButton* button = (NSButton*)get_handle();
        [button sizeToFit];
        NSRect frame = button.frame;
        __set_size_noevent(xaml::get_size(frame.size));
    }
}
