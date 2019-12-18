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

    string_t window::get_title() const
    {
        return gtk_window_get_title(GTK_WINDOW(get_widget()));
    }

    void window::set_title(string_view_t value)
    {
        gtk_window_set_title(GTK_WINDOW(get_widget()), value.data());
    }
} // namespace xaml
