#import <cocoa/XamlEntryDelegate.h>
#include <cocoa/nsstring.hpp>
#include <shared/atomic_guard.hpp>
#include <shared/entry.hpp>
#include <xaml/ui/controls/entry.h>

@implementation XamlEntryDelegate
- (void)controlTextDidChange:(NSNotification*)obj
{
    xaml_entry_internal* ptr = (xaml_entry_internal*)self.classPointer;
    ptr->on_changed();
}
@end

using namespace std;

xaml_result xaml_entry_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        NSTextField* textField = [NSTextField new];
        textField.bezeled = YES;
        textField.drawsBackground = YES;
        textField.editable = YES;
        textField.selectable = YES;
        XamlEntryDelegate* delegate = [[XamlEntryDelegate alloc] initWithClassPointer:this];
        textField.delegate = delegate;
        m_handle = textField;
        m_delegate = delegate;
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_alignment());
    }
    return set_rect(region);
}

xaml_result xaml_entry_internal::draw_text() noexcept
{
    NSTextField* textField = (NSTextField*)m_handle;
    NSString* ns_title;
    XAML_RETURN_IF_FAILED(get_NSString(m_text, &ns_title));
    textField.stringValue = ns_title;
    return XAML_S_OK;
}

xaml_result xaml_entry_internal::draw_alignment() noexcept
{
    NSTextField* textField = (NSTextField*)m_handle;
    NSTextAlignment align;
    switch (m_text_halignment)
    {
    case xaml_halignment_left:
        align = NSTextAlignmentLeft;
        break;
    case xaml_halignment_center:
        align = NSTextAlignmentCenter;
        break;
    case xaml_halignment_right:
        align = NSTextAlignmentRight;
        break;
    default:
        align = NSTextAlignmentNatural;
        break;
    }
    textField.alignment = align;
    return XAML_S_OK;
}

void xaml_entry_internal::on_changed() noexcept
{
    xaml_atomic_guard guard{ m_text_changing };
    guard.test_and_set();
    NSTextField* textField = (NSTextField*)m_handle;
    xaml_ptr<xaml_string> str;
    XAML_ASSERT_SUCCEEDED(xaml_string_new(textField.stringValue.UTF8String, &str));
    XAML_ASSERT_SUCCEEDED(set_text(str));
    XAML_ASSERT_SUCCEEDED(parent_redraw());
}
