#include <xaml/ui/controls/radio_box.hpp>

using namespace std;

namespace xaml
{
    void radio_box::__draw(rectangle const& region)
    {
        button::__draw(region);
        NSButton* button = (NSButton*)get_handle();
        [button setButtonType:NSButtonTypeRadio];
        draw_checked();
    }

    void radio_box::draw_checked()
    {
        NSButton* button = (NSButton*)get_handle();
        button.state = m_is_checked ? NSControlStateValueOn : NSControlStateValueOff;
    }
}
