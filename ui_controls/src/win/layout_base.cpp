#include <xaml/ui/controls/layout_base.hpp>

using namespace std;

namespace xaml
{
    void layout_base::__draw(rectangle const& region)
    {
        set_handle(get_parent()->get_handle());
        SendMessage(get_handle(), WM_SETREDRAW, FALSE, 0);
        __draw_impl(region);
        SendMessage(get_handle(), WM_SETREDRAW, TRUE, 0);
    }
} // namespace xaml
