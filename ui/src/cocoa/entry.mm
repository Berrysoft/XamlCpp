#import <cocoa/XamlEntryDelegate.h>
#include <cocoa/drawing.hpp>
#include <xaml/ui/entry.hpp>

@implementation XamlEntryDelegate
- (void)controlTextDidChange:(NSNotification*)obj
{
    xaml::entry* ptr = (xaml::entry*)self.classPointer;
    ptr->__on_changed();
}
@end

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
            set_handle(textField);
            __set_delegate(delegate);
        }
        rectangle real = region - get_margin();
        NSTextField* textField = (NSTextField*)get_handle();
        NSView* view = get_parent()->get_handle();
        NSRect parent_frame = [view frame];
        NSRect frame = textField.frame;
        frame.origin = { real.x, parent_frame.size.height - real.height - real.y };
        frame.size = { real.width, real.height };
        textField.frame = frame;
        __set_size_noevent({ real.width, real.height });
        draw_text();
        draw_alignment();
    }

    void entry::draw_size()
    {
        NSTextField* textField = (NSTextField*)get_handle();
        NSRect frame = textField.frame;
        frame.size = get_NSSize(get_size());
        textField.frame = frame;
    }

    void entry::draw_text()
    {
        NSTextField* textField = (NSTextField*)get_handle();
        NSString* ns_title = [NSString stringWithUTF8String:m_text.c_str()];
        textField.stringValue = ns_title;
    }

    void entry::draw_alignment()
    {
        NSTextField* textField = (NSTextField*)get_handle();
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
        NSTextField* textField = (NSTextField*)get_handle();
        m_text_changed(*this, [textField.stringValue UTF8String]);
    }

    void entry::__size_to_fit()
    {
        NSTextField* textField = (NSTextField*)get_handle();
        [textField sizeToFit];
        NSRect frame = textField.frame;
        __set_size_noevent(xaml::get_size(frame.size));
    }
}
