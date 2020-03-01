#include <xaml/ui/controls/label.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_drawing.hpp>

using namespace std;

namespace xaml
{
    void label::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            NSTextField* textField = [NSTextField new];
            textField.bezeled = NO;
            textField.drawsBackground = NO;
            textField.editable = NO;
            textField.selectable = NO;
            auto h = make_shared<native_control>();
            h->handle = textField;
            set_handle(h);
        }
        rectangle real = region - get_margin();
        __set_rect(real);
        draw_text();
        draw_alignment();
    }

    void label::draw_text()
    {
        NSTextField* textField = (NSTextField*)get_handle()->handle;
        NSString* ns_title = [NSString stringWithUTF8String:m_text.c_str()];
        textField.stringValue = ns_title;
    }

    void label::draw_alignment()
    {
        NSTextField* textField = (NSTextField*)get_handle()->handle;
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
}
