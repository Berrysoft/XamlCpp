#include <xaml/ui/window.hpp>

namespace xaml
{
    void window::create()
    {
        widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    }

    window::~window()
    {
        gtk_window_close(GTK_WINDOW(widget));
    }

    void window::show()
    {
        if (!widget)
        {
            create();
        }
        gtk_widget_show_all(widget);
    }
} // namespace xaml
