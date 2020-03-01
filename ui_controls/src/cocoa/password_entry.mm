#import <cocoa/XamlEntryDelegate.h>
#include <xaml/ui/controls/password_entry.hpp>
#include <xaml/ui/native_control.hpp>

using namespace std;

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
            XamlEntryDelegate* delegate = [[XamlEntryDelegate alloc] initWithClassPointer:this];
            textField.delegate = delegate;
            auto h = make_shared<native_control>();
            h->handle = textField;
            h->delegate = delegate;
            set_handle(h);
        }
        entry::__draw(region);
        draw_password_char();
    }

    void password_entry::draw_password_char()
    {
    }
}
