#include <xaml/ui/application.hpp>
#include <xaml/ui/window.hpp>

namespace xaml
{
    window::window() : container()
    {
    }

    window::~window()
    {
        gtk_window_close(GTK_WINDOW(get_handle()));
    }

    void window::draw(rectangle const& region)
    {
        if (!get_handle())
        {
            set_handle(gtk_window_new(GTK_WINDOW_TOPLEVEL));
            application::current()->wnd_num++;
            g_signal_connect(
                G_OBJECT(get_handle()), "destroy",
                G_CALLBACK(+[](GtkWidget* w, gpointer arg) { application::current()->decrease_quit(); }),
                this);
        }
        gtk_window_resize(GTK_WINDOW(get_handle()), get_width(), get_height());
        gtk_window_move(GTK_WINDOW(get_handle()), get_x(), get_y());
        gtk_window_set_title(GTK_WINDOW(get_handle()), m_title.data());
        if (get_child())
        {
            get_child()->draw({ 0, 0, get_width(), get_height() });
            gtk_container_add(GTK_CONTAINER(get_handle()), get_child()->get_handle());
        }
        gtk_widget_show_all(get_handle());
    }

    void window::show()
    {
        draw({ 0, 0, 0, 0 });
    }
} // namespace xaml
