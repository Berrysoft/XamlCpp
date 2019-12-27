#import <internal/cocoa/global.h>

#import <internal/cocoa/XamlButtonDelegate.h>
#include <internal/cocoa/drawing.hpp>
#include <xaml/ui/button.hpp>

#ifdef XAML_UI_USE_GNUSTEP
constexpr NSBezelStyle NSBezelStyleRounded = NSRoundedBezelStyle;
#endif // XAML_UI_USE_GNUSTEP

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
    xaml::button* ptr = (xaml::button*)self.classPointer;
    ptr->__on_action();
}
@end

using namespace std;

namespace xaml
{
    button::button() : common_control()
    {
        add_text_changed([this](button const&, string_view_t) { if(get_handle()) draw_text(); });
        add_size_changed([this](control const&, size) { if(get_handle()) draw_size(); });
        add_is_default_changed([this](button const&, bool) { if (get_handle()) draw_default(); });
    }

    button::~button()
    {
    }

    void button::__draw(const rectangle& region)
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
        NSWindow* window = (NSWindow*)get_parent()->get_handle();
        NSRect parent_frame = [window.contentView frame];
        NSRect frame = button.frame;
        frame.origin = { real.x, parent_frame.size.height - real.height - real.y };
        button.frame = frame;
        set_size({ real.width, real.height });
        draw_text();
        draw_default();
    }

    void button::draw_text()
    {
        NSButton* button = (NSButton*)get_handle();
        NSString* ns_title = [NSString stringWithUTF8String:m_text.c_str()];
        button.title = ns_title;
    }

    void button::draw_size()
    {
        NSButton* button = (NSButton*)get_handle();
        NSRect frame = button.frame;
        frame.size = get_NSSize(get_size());
        button.frame = frame;
    }

    void button::draw_default()
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

    void button::__on_action()
    {
        m_click(*this);
    }
}
