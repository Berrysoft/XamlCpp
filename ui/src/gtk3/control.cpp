#include <xaml/ui/control.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_drawing.hpp>

using namespace std;

namespace xaml
{
    void control::__size_to_fit()
    {
        int width, height;
        gtk_widget_get_preferred_width(get_handle()->handle, NULL, &width);
        gtk_widget_get_preferred_height(get_handle()->handle, NULL, &height);
        __set_size_noevent({ (double)width, (double)height });
    }

    void control::draw_size()
    {
        auto [rw, rh] = to_native<tuple<gint, gint>>(get_size());
        gtk_widget_set_size_request(get_handle()->handle, rw, rh);
    }
} // namespace xaml
