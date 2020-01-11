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
        entry::__draw(region);
        draw_password_char();
    }

    void password_entry::draw_password_char()
    {
    }
}
