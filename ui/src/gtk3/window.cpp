#include <cmath>
#include <xaml/ui/application.hpp>
#include <xaml/ui/window.hpp>

using namespace std;

namespace xaml
{
    window::window() : container()
    {
        add_location_changed([this](window const&, point) { if (showed && !resizing) draw({ 0, 0, 0, 0 }); });
        add_size_changed([this](control const&, size) { if (showed && !resizing) draw({ 0, 0, 0, 0 }); });
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
            g_signal_connect(
                G_OBJECT(get_handle()), "size-allocate",
                G_CALLBACK(window::on_size_allocate),
                this);
        }
        if (!resizing)
        {
            gtk_window_resize(GTK_WINDOW(get_handle()), (int)round(get_width()), (int)round(get_height()));
            gtk_window_move(GTK_WINDOW(get_handle()), get_x(), get_y());
        }
        gtk_window_set_title(GTK_WINDOW(get_handle()), m_title.data());
        if (get_child())
        {
            get_child()->draw({ 0, 0, get_width(), get_height() });
        }
        gtk_widget_show_all(get_handle());
    }

    void window::show()
    {
        draw({ 0, 0, 0, 0 });
        showed = true;
    }

    gboolean window::invoke_draw(gpointer data)
    {
        window* self = (window*)data;
        self->draw({ 0, 0, 0, 0 });
        self->resizing = false;
        return false;
    }

    void window::on_size_allocate(GtkWidget* widget, GdkRectangle* allocation, gpointer data)
    {
        window* self = (window*)data;
        if (self->showed && !self->resizing)
        {
            self->resizing = true;
            int x, y;
            gtk_window_get_position(GTK_WINDOW(widget), &x, &y);
            self->set_location({ (double)x, (double)y });
            int width, height;
            gtk_window_get_size(GTK_WINDOW(widget), &width, &height);
            self->set_size({ (double)width, (double)height });
            gdk_threads_add_idle(window::invoke_draw, data);
        }
    }
} // namespace xaml
