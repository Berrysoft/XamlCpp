#include <cocoa/drawing.hpp>
#include <xaml/ui/password_entry.hpp>

namespace xaml
{
    void password_entry::__draw(const rectangle& region)
    {
        if (!get_handle())
        {
            NSSecureTextField* textField = [NSSecureTextField new];
            textField.bezeled = YES;
            textField.drawsBackground = YES;
            textField.editable = YES;
            textField.selectable = YES;
            set_handle(textField);
        }
        rectangle real = region - get_margin();
        NSSecureTextField* textField = (NSSecureTextField*)get_handle();
        NSView* view = get_parent()->get_handle();
        NSRect parent_frame = [view frame];
        NSRect frame = textField.frame;
        frame.origin = { real.x, parent_frame.size.height - real.height - real.y };
        frame.size = { real.width, real.height };
        textField.frame = frame;
        __set_size_noevent({ real.width, real.height });
        draw_text();
        draw_password_char();
    }

    void password_entry::draw_size()
    {
        NSSecureTextField* textField = (NSSecureTextField*)get_handle();
        NSRect frame = textField.frame;
        frame.size = get_NSSize(get_size());
        textField.frame = frame;
    }

    void password_entry::draw_text()
    {
        NSSecureTextField* textField = (NSSecureTextField*)get_handle();
        NSString* ns_title = [NSString stringWithUTF8String:m_text.c_str()];
        textField.stringValue = ns_title;
    }

    void password_entry::draw_password_char()
    {
    }

    void password_entry::__size_to_fit()
    {
        NSSecureTextField* textField = (NSSecureTextField*)get_handle();
        [textField sizeToFit];
        NSRect frame = textField.frame;
        __set_size_noevent(xaml::get_size(frame.size));
    }
}
