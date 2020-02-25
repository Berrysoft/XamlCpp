#include <xaml/ui/application.hpp>
#include <xaml/ui/controls/layout_base.hpp>

using namespace std;

namespace xaml
{
    void layout_base::__draw(rectangle const& region)
    {
        if (auto sparent = get_parent().lock())
        {
            set_handle(sparent->get_handle());
            SendMessage(get_handle(), WM_SETREDRAW, FALSE, 0);
            __draw_impl(region, [this](shared_ptr<control> c, rectangle const&) {
                SendMessage(c->get_handle(), WM_SETFONT, (WPARAM)application::current()->__default_font(get_handle()), TRUE);
            });
            SendMessage(get_handle(), WM_SETREDRAW, TRUE, 0);
        }
    }
} // namespace xaml
