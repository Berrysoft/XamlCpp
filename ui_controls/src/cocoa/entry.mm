#import <cocoa/XamlEntryDelegate.h>
#include <cocoa/strings.hpp>
#include <xaml/ui/controls/entry.hpp>
#include <xaml/ui/native_control.hpp>

@implementation XamlEntryDelegate
- (void)controlTextDidChange:(NSNotification*)obj
{
    xaml::entry* ptr = (xaml::entry*)self.classPointer;
    ptr->__on_changed();
}
@end

using namespace std;

namespace xaml
{
    void entry::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            NSTextField* textField = [NSTextField new];
            textField.bezeled = YES;
            textField.drawsBackground = YES;
            textField.editable = YES;
            textField.selectable = YES;
            XamlEntryDelegate* delegate = [[XamlEntryDelegate alloc] initWithClassPointer:this];
            textField.delegate = delegate;
            auto h = make_shared<native_control>();
            h->handle = textField;
            h->delegate = delegate;
            set_handle(h);
            draw_visible();
            draw_text();
            draw_alignment();
        }
        __set_rect(region);
    }

    void entry::draw_text()
    {
        NSTextField* textField = (NSTextField*)get_handle()->handle;
        NSString* ns_title = to_native(m_text);
        textField.stringValue = ns_title;
    }

    void entry::draw_alignment()
    {
        NSTextField* textField = (NSTextField*)get_handle()->handle;
        NSTextAlignment align;
        switch (m_text_halignment)
        {
        case halignment_t::left:
            align = NSTextAlignmentLeft;
            break;
        case halignment_t::center:
            align = NSTextAlignmentCenter;
            break;
        case halignment_t::right:
            align = NSTextAlignmentRight;
            break;
        default:
            align = NSTextAlignmentNatural;
            break;
        }
        textField.alignment = align;
    }

    void entry::__on_changed()
    {
        NSTextField* textField = (NSTextField*)get_handle()->handle;
        set_text(from_native(textField.stringValue));
    }
}
