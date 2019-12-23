#include <internal/gtk3/drawing.hpp>
#include <xaml/ui/application.hpp>
#include <xaml/ui/window.hpp>

using namespace std;

namespace xaml
{
    window::window() : container(), m_resizable(true)
    {
        add_title_changed([this](window const&, string_view_t) { if (get_handle()) draw_title(); });
        add_location_changed([this](window const&, point) { if (get_handle() && !resizing) draw({}); });
        add_size_changed([this](control const&, size) { if (get_handle() && !resizing) draw({}); });
        add_resizable_changed([this](control const&, bool) { if(get_handle()) draw_resizable(); });
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
                G_CALLBACK(window::on_destroy),
                this);
            g_signal_connect(
                G_OBJECT(get_handle()), "configure-event",
                G_CALLBACK(window::on_configure_event),
                this);
        }
        if (!resizing)
        {
            gtk_window_resize(GTK_WINDOW(get_handle()), get_rwidth(get_width()), get_rheight(get_height()));
            gtk_window_set_default_size(GTK_WINDOW(get_handle()), get_rwidth(get_width()), get_rheight(get_height()));
            gtk_window_move(GTK_WINDOW(get_handle()), get_x(), get_y());
        }
        draw_title();
        if (get_child())
        {
            draw_child();
        }
        gtk_widget_show_all(get_handle());
        draw_resizable();
    }

    void window::draw_title()
    {
        gtk_window_set_title(GTK_WINDOW(get_handle()), m_title.data());
    }

    void window::draw_child()
    {
        get_child()->draw(get_client_region());
    }

    void window::draw_resizable()
    {
        gtk_window_set_resizable(GTK_WINDOW(get_handle()), m_resizable ? TRUE : FALSE);
    }

    void window::show()
    {
        draw({});
    }

    rectangle window::get_client_region() const
    {
        gint width, height;
        gtk_window_get_size(GTK_WINDOW(get_handle()), &width, &height);
        return { 0, 0, (double)width, (double)height };
    }

    gboolean window::invoke_draw(gpointer data)
    {
        window* self = (window*)data;
        self->draw({});
        self->resizing = false;
        return false;
    }

    void window::on_destroy(GtkWidget* w, gpointer arg)
    {
        if (!(--application::current()->wnd_num)) gtk_main_quit();
    }

    gboolean window::on_configure_event(GtkWidget* widget, GdkEvent* event, gpointer data)
    {
        window* self = (window*)data;
        if (event->type == GDK_CONFIGURE && self->get_handle() && !self->resizing)
        {
            self->resizing = true;
            self->set_location({ (double)event->configure.x, (double)event->configure.y });
            self->set_size({ (double)event->configure.width, (double)event->configure.height });
            gdk_threads_add_idle(window::invoke_draw, data);
        }
        return FALSE;
    }
} // namespace xaml
