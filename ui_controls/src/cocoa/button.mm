#import <cocoa/XamlButtonDelegate.h>
#include <xaml/ui/controls/button.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_drawing.hpp>

@implementation XamlButtonDelegate : XamlDelegate
- (void)onAction
{
    xaml::button* ptr = (xaml::button*)self.classPointer;
    ptr->__on_action();
}
@end

using namespace std;

namespace xaml
{
    void button::__draw(const rectangle& region)
    {
        if (!get_handle())
        {
            XamlButtonDelegate* delegate = [[XamlButtonDelegate alloc] initWithClassPointer:this];
            NSButtonCell* button = [NSButtonCell new];
            button.target = delegate;
            button.action = @selector(onAction);
            [button setBezelStyle:NSBezelStyleRounded];
            NSMatrix* matrix = [[NSMatrix alloc] initWithFrame:NSMakeRect(0, 0, 1, 1)
                                                          mode:NSTrackModeMatrix
                                                     prototype:(NSCell*)button
                                                  numberOfRows:1
                                               numberOfColumns:1];
            matrix.autorecalculatesCellSize = YES;
            auto h = make_shared<native_control>();
            h->handle = matrix;
            h->delegate = delegate;
            set_handle(h);
            draw_visible();
            draw_text();
            draw_default();
        }
        __set_rect(region);
        NSMatrix* matrix = (NSMatrix*)get_handle()->handle;
        matrix.cellSize = matrix.frame.size;
    }

    void button::draw_text()
    {
        NSMatrix* matrix = (NSMatrix*)get_handle()->handle;
        NSButtonCell* button = (NSButtonCell*)[matrix.cells objectAtIndex:0];
        NSString* ns_title = [NSString stringWithUTF8String:m_text.c_str()];
        button.title = ns_title;
    }

    void button::draw_size()
    {
        NSMatrix* matrix = (NSMatrix*)get_handle()->handle;
        NSRect frame = matrix.frame;
        frame.size = to_native<NSSize>(get_size());
        matrix.frame = frame;
        matrix.cellSize = frame.size;
    }

    void button::draw_default()
    {
        NSMatrix* matrix = (NSMatrix*)get_handle()->handle;
        NSButtonCell* button = (NSButtonCell*)[matrix.cells objectAtIndex:0];
        if (m_is_default)
        {
            button.keyEquivalent = @"\r";
            button.highlighted = YES;
        }
        else
        {
            button.keyEquivalent = @"";
            button.highlighted = NO;
        }
    }

    void button::__on_action()
    {
        m_click(shared_from_this<button>());
    }
}
