#import <cocoa/XamlTextBoxDelegate.h>
#include <cocoa/nsstring.hpp>
#include <shared/atomic_guard.hpp>
#include <shared/text_box.hpp>
#include <xaml/ui/controls/text_box.h>

@implementation XamlTextBoxDelegate
- (void)textDidChange:(NSNotification*)obj
{
    xaml_text_box_internal* ptr = (xaml_text_box_internal*)self.classPointer;
    ptr->on_changed();
}
@end

using namespace std;

xaml_result xaml_text_box_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        NSTextView* textView = [NSTextView new];
        textView.richText = NO;
        textView.editable = YES;
        textView.selectable = YES;
        XamlTextBoxDelegate* delegate = [[XamlTextBoxDelegate alloc] initWithClassPointer:this];
        textView.delegate = delegate;
        m_handle = textView;
        m_delegate = delegate;
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
    }
    return set_rect(region);
}

xaml_result xaml_text_box_internal::draw_text() noexcept
{
    NSTextView* textView = (NSTextView*)m_handle;
    NSString* ns_title;
    XAML_RETURN_IF_FAILED(get_NSString(m_text, &ns_title));
    textView.string = ns_title;
    return XAML_S_OK;
}

void xaml_text_box_internal::on_changed() noexcept
{
    xaml_atomic_guard guard{ m_text_changing };
    guard.test_and_set();
    NSTextView* textView = (NSTextView*)m_handle;
    xaml_ptr<xaml_string> str;
    XAML_ASSERT_SUCCEEDED(xaml_string_new(textView.string.UTF8String, &str));
    XAML_ASSERT_SUCCEEDED(set_text(str));
    XAML_ASSERT_SUCCEEDED(parent_redraw());
}
