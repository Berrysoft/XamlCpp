#include <xaml/ui/controls/check_box.hpp>
#include <xaml/ui/native_control.hpp>

using namespace std;

namespace xaml
{
    void check_box::__draw(rectangle const& region)
    {
        bool new_draw = !get_handle();
        button::__draw(region);
        if (new_draw)
        {
            NSMatrix* matrix = (NSMatrix*)get_handle()->handle;
            NSButtonCell* button = (NSButtonCell*)[matrix.cells objectAtIndex:0];
            [button setButtonType:NSButtonTypeSwitch];
            draw_checked();
		}
    }

    void check_box::draw_checked()
    {
        NSMatrix* matrix = (NSMatrix*)get_handle()->handle;
        NSButtonCell* button = (NSButtonCell*)[matrix.cells objectAtIndex:0];
        button.state = m_is_checked ? NSControlStateValueOn : NSControlStateValueOff;
    }

    void check_box::__on_state_changed()
    {
        NSMatrix* matrix = (NSMatrix*)get_handle()->handle;
        NSButtonCell* button = (NSButtonCell*)[matrix.cells objectAtIndex:0];
        set_is_checked(button.state == NSControlStateValueOn);
    }
}
