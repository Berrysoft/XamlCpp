#include <xaml/ui/application.hpp>
#include <xaml/ui/controls/layout_base.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/win/dpi.h>

using namespace std;

namespace xaml
{
    void layout_base::__draw(rectangle const& region)
    {
        if (auto sparent = get_parent().lock())
        {
            set_handle(sparent->get_handle());
            HFONT def_font = (HFONT)application::current()->__default_font(XamlGetDpiForWindow(get_handle()->handle));
            __draw_impl(region, [this, def_font](shared_ptr<control> c, rectangle const& region) {
                SendMessage(c->get_handle()->handle, WM_SETFONT, (WPARAM)def_font, FALSE);
            });
        }
    }
} // namespace xaml
