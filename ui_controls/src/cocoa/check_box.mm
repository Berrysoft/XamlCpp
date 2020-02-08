#include <xaml/ui/controls/check_box.hpp>

using namespace std;

namespace xaml
{
    check_box::check_box() : button()
    {
        add_is_checked_changed([this](check_box const&, bool) { if(get_handle()) draw_checked(); });
    }

    check_box::~check_box() {}

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
}
