#include <xaml/ui/application.hpp>
#include <xaml/ui/window.hpp>

namespace xaml
{
    window::window() : container()
    {
        set_widget(gtk_window_new(GTK_WINDOW_TOPLEVEL));
        application::current()->wnd_num++;
        g_signal_connect(
            G_OBJECT(get_widget()), "destroy",
            G_CALLBACK(+[](GtkWidget* w, gpointer arg) { application::current()->decrease_quit(); }),
            this);
    }

    window::~window()
    {
        gtk_window_close(GTK_WINDOW(get_widget()));
        gtk_widget_destroy(get_widget());
    }

    void window::show()
    {
        gtk_widget_show_all(get_widget());
    }
} // namespace xaml
