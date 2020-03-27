#include <xaml/ui/controls/check_box.hpp>

using namespace std;

namespace xaml
{
    check_box::check_box() : button()
    {
        add_is_checked_changed([this](shared_ptr<check_box>, bool) { if(get_handle()) draw_checked(); });
#ifdef XAML_UI_COCOA
        add_click([this](shared_ptr<button>) { if(get_handle()) __on_state_changed(); });
#endif // XAML_UI_COCOA
    }

    check_box::~check_box() {}
} // namespace xaml
