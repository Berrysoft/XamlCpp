#include <xaml/ui/controls/check_box.hpp>

namespace xaml
{
    check_box::check_box() : button()
    {
        add_is_checked_changed([this](check_box const&, bool) { if(get_handle()) draw_checked(); });
#ifdef XAML_UI_COCOA
        add_click([this](button const&) { if(get_handle()) __on_state_changed(); });
#endif // XAML_UI_COCOA
    }

    check_box::~check_box() {}
} // namespace xaml
