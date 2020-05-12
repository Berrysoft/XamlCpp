#import <cocoa/XamlEntryDelegate.h>
#include <shared/password_entry.hpp>
#include <xaml/ui/controls/password_entry.h>

using namespace std;

xaml_result xaml_password_entry_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        NSSecureTextField* textField = [NSSecureTextField new];
        textField.bezeled = YES;
        textField.drawsBackground = YES;
        textField.editable = YES;
        textField.selectable = YES;
        XamlEntryDelegate* delegate = [[XamlEntryDelegate alloc] initWithClassPointer:this];
        textField.delegate = delegate;
        m_handle = textField;
        m_delegate = delegate;
        XAML_RETURN_IF_FAILED(draw_password_char());
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_alignment());
    }
    return xaml_entry_internal::draw(region);
}

xaml_result xaml_password_entry_internal::draw_password_char() noexcept
{
    return XAML_S_OK;
}
