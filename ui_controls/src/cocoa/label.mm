#include <cocoa/nsstring.hpp>
#include <shared/label.hpp>
#include <xaml/ui/controls/label.h>

using namespace std;

xaml_result xaml_label_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        NSTextField* textField = [NSTextField new];
        textField.bezeled = NO;
        textField.drawsBackground = NO;
        textField.editable = NO;
        textField.selectable = NO;
        m_handle = textField;
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_alignment());
    }
    XAML_RETURN_IF_FAILED(set_rect(region));
    return XAML_S_OK;
}

xaml_result xaml_label_internal::draw_text() noexcept
{
    NSTextField* textField = (NSTextField*)m_handle;
    NSString* ns_title;
    XAML_RETURN_IF_FAILED(get_NSString(m_text, &ns_title));
    textField.stringValue = ns_title;
    return XAML_S_OK;
}

xaml_result xaml_label_internal::draw_alignment() noexcept
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
