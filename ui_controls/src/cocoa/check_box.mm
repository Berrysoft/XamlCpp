#include <xaml/ui/controls/check_box.hpp>

using namespace std;

namespace xaml
{
    void check_box::__draw(rectangle const& region)
    {
        button::__draw(region);
        NSButton* button = (NSButton*)get_handle();
        [button setButtonType:NSButtonTypeSwitch];
        draw_checked();
    }

    void check_box::draw_checked()
    {
        NSButton* button = (NSButton*)get_handle();
        button.state = m_is_checked ? NSControlStateValueOn : NSControlStateValueOff;
    }

    void check_box::__on_state_changed()
    {
        NSButton* button = (NSButton*)get_handle();
        set_is_checked(button.state == NSControlStateValueOn);
	}
}
