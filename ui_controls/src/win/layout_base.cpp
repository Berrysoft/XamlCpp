#include <xaml/ui/application.hpp>
#include <xaml/ui/controls/layout_base.hpp>
#include <xaml/ui/native_control.hpp>

using namespace std;

namespace xaml
{
    void layout_base::__draw(rectangle const& region)
    {
        if (auto sparent = get_parent().lock())
        {
            set_handle(sparent->get_handle());
            __draw_impl(region, [this](shared_ptr<control> c, rectangle const&) {
                SendMessage(c->get_handle()->handle, WM_SETFONT, (WPARAM)application::current()->__default_font(get_handle()->handle), TRUE);
            });
        }
    }
} // namespace xaml
