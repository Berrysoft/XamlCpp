#include <xaml/ui/controls/combo_box.hpp>

using namespace std;

namespace xaml
{
    void combo_box::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            NSComboBox* combo = [[NSComboBox alloc] init];
            combo.bezeled = YES;
            set_handle(combo);
            draw_items();
        }
        rectangle real = region - get_margin();
        __set_rect(real);
        draw_sel();
        draw_editable();
    }

    void combo_box::draw_size()
    {
        NSComboBox* combo = (NSComboBox*)get_handle();
        NSRect frame = combo.frame;
        frame.size = to_native<NSSize>(get_size());
        combo.frame = frame;
    }

    void combo_box::draw_text()
    {
        NSComboBox* combo = (NSComboBox*)get_handle();
        if (m_text)
        {
            NSString* ns_title = [NSString stringWithUTF8String:m_text->c_str()];
            combo.stringValue = ns_title;
        }
        else
        {
            if (m_sel_id < 0 || m_sel_id >= m_items.size())
            {
                combo.stringValue = @"";
            }
            else
            {
                NSString* ns_title = [NSString stringWithUTF8String:m_items[m_sel_id].c_str()];
                combo.stringValue = ns_title;
            }
        }
    }

    void combo_box::draw_items()
    {
        NSComboBox* combo = (NSComboBox*)get_handle();
        for (string_t const& item : m_items)
        {
            NSString* ns_item = [NSString stringWithUTF8String:item.c_str()];
            [combo addItemWithObjectValue:ns_item];
        }
    }

    void combo_box::draw_sel()
    {
        NSComboBox* combo = (NSComboBox*)get_handle();
        [combo selectItemAtIndex:(NSInteger)m_sel_id];
    }

    void combo_box::draw_editable()
    {
        NSComboBox* combo = (NSComboBox*)get_handle();
        if (m_is_editable)
        {
            combo.drawsBackground = YES;
            combo.editable = YES;
            combo.selectable = YES;
		}
        else
        {
            combo.drawsBackground = NO;
            combo.editable = NO;
            combo.selectable = NO;
		}
    }

    void combo_box::__size_to_fit()
    {
        NSComboBox* combo = (NSComboBox*)get_handle();
        [combo sizeToFit];
        NSRect frame = combo.frame;
        __set_size_noevent(from_native(frame.size));
    }
}
