#include <xaml/ui/control.hpp>

namespace xaml
{
    void control::__size_to_fit()
    {
        int width, height;
        gtk_widget_get_preferred_width(get_handle(), NULL, &width);
        gtk_widget_get_preferred_height(get_handle(), NULL, &height);
        __set_size_noevent({ (double)width, (double)height });
    }
} // namespace xaml
