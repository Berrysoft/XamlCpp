#import <cocoa/XamlComboBoxDelegate.h>
#include <cocoa/nsstring.hpp>
#include <shared/combo_box.hpp>
#include <xaml/ui/controls/combo_box.h>
#include <xaml/ui/drawing_conv.hpp>

@implementation XamlComboBoxDelegate
- (void)comboBoxSelectionDidChange:(NSNotification*)obj
{
    xaml_combo_box_internal* ptr = (xaml_combo_box_internal*)self.classPointer;
    ptr->on_changed();
}
@end

using namespace std;

xaml_result xaml_combo_box_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        NSComboBox* combo = [NSComboBox new];
        combo.bezeled = YES;
        XamlComboBoxDelegate* delegate = [[XamlComboBoxDelegate alloc] initWithClassPointer:this];
        combo.delegate = delegate;
        m_handle = combo;
        m_delegate = delegate;
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_items());
        XAML_RETURN_IF_FAILED(draw_sel());
        XAML_RETURN_IF_FAILED(draw_editable());
    }
    return set_rect(region);
}

xaml_result xaml_combo_box_internal::draw_text() noexcept
{
    NSComboBox* combo = (NSComboBox*)m_handle;
    if (m_text)
    {
        char const* data;
        XAML_RETURN_IF_FAILED(m_text->get_data(&data));
        combo.stringValue = [NSString stringWithUTF8String:data];
    }
    else
    {
        int32_t size;
        XAML_RETURN_IF_FAILED(m_items->get_size(&size));
        if (m_sel_id < 0 || m_sel_id >= size)
        {
            combo.stringValue = @"";
        }
        else
        {
            xaml_ptr<xaml_object> item;
            XAML_RETURN_IF_FAILED(m_items->get_at(m_sel_id, &item));
            if (auto str = item.query<xaml_string>())
            {
                char const* data;
                XAML_RETURN_IF_FAILED(str->get_data(&data));
                combo.stringValue = [NSString stringWithUTF8String:data];
            }
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::draw_items() noexcept
{
    NSComboBox* combo = (NSComboBox*)m_handle;
    XAML_FOREACH_START(item, m_items);
    {
        xaml_ptr<xaml_string> s = item.query<xaml_string>();
        if (s)
        {
            char const* data;
            XAML_RETURN_IF_FAILED(s->get_data(&data));
            [combo addItemWithObjectValue:[NSString stringWithUTF8String:data]];
        }
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::draw_sel() noexcept
{
    NSComboBox* combo = (NSComboBox*)m_handle;
    [combo selectItemAtIndex:(NSInteger)m_sel_id];
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::draw_editable() noexcept
{
    NSComboBox* combo = (NSComboBox*)m_handle;
    combo.drawsBackground = m_is_editable;
    combo.editable = m_is_editable;
    combo.selectable = m_is_editable;
    return XAML_S_OK;
}

void xaml_combo_box_internal::on_changed() noexcept
{
    NSComboBox* combo = (NSComboBox*)m_handle;
    XAML_ASSERT_SUCCEEDED(set_sel_id(combo.indexOfSelectedItem));
    xaml_ptr<xaml_string> str;
    XAML_ASSERT_SUCCEEDED(xaml_string_new(combo.stringValue.UTF8String, &str));
    XAML_ASSERT_SUCCEEDED(set_text(str));
}

xaml_result xaml_combo_box_internal::insert_item(int32_t index, xaml_ptr<xaml_object> const& value) noexcept
{
    NSComboBox* combo = (NSComboBox*)m_handle;
    xaml_ptr<xaml_string> s = value.query<xaml_string>();
    NSString* text;
    XAML_RETURN_IF_FAILED(get_NSString(s, &text));
    [combo insertItemWithObjectValue:text
                             atIndex:(NSInteger)index];
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::remove_item(int32_t index) noexcept
{
    NSComboBox* combo = (NSComboBox*)m_handle;
    [combo removeItemAtIndex:(NSInteger)index];
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::clear_items() noexcept
{
    NSComboBox* combo = (NSComboBox*)m_handle;
    [combo removeAllItems];
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::replace_item(int32_t index, xaml_ptr<xaml_object> const& value) noexcept
{
    XAML_RETURN_IF_FAILED(remove_item(index));
    XAML_RETURN_IF_FAILED(insert_item(index, value));
    return XAML_S_OK;
}
