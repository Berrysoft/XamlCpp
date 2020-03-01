#include <xaml/ui/controls/radio_box.hpp>
#include <xaml/ui/native_control.hpp>

using namespace std;

namespace xaml
{
    void radio_box::__draw(rectangle const& region)
    {
        button::__draw(region);
        NSMatrix* matrix = (NSMatrix*)get_handle()->handle;
        NSButtonCell* button = (NSButtonCell*)[matrix.cells objectAtIndex:0];
        [button setButtonType:NSButtonTypeRadio];
        draw_checked();
    }

    void radio_box::draw_checked()
    {
        NSMatrix* matrix = (NSMatrix*)get_handle()->handle;
        NSButtonCell* button = (NSButtonCell*)[matrix.cells objectAtIndex:0];
        button.state = m_is_checked ? NSControlStateValueOn : NSControlStateValueOff;
    }

    void radio_box::__on_state_changed()
    {
        NSMatrix* matrix = (NSMatrix*)get_handle()->handle;
        NSButtonCell* button = (NSButtonCell*)[matrix.cells objectAtIndex:0];
        set_is_checked(button.state == NSControlStateValueOn);
    }
}
