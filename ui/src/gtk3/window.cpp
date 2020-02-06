#include <gtk3/resources.hpp>
#include <shared/atomic_guard.hpp>
#include <xaml/ui/application.hpp>
#include <xaml/ui/window.hpp>

using namespace std;

namespace xaml
{
    window::window() : container(), m_resizable(true)
    {
        add_title_changed([this](window const&, string_view_t) { if (get_handle()) draw_title(); });
        add_location_changed([this](window const&, point) { if (get_handle() && !m_resizing) __draw({}); });
        add_size_changed([this](control const&, size) { if (get_handle() && !m_resizing) __draw({}); });
        add_resizable_changed([this](control const&, bool) { if(get_handle()) draw_resizable(); });
    }

    window::~window()
    {
        gtk_window_close(GTK_WINDOW(get_handle()));
    }

    void window::__draw(rectangle const& region)
    {
        bool new_run = !get_handle();
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
        {
            atomic_guard guard{ m_resizing };
            if (!guard.exchange(true))
            {
                auto [rw, rh] = to_native<tuple<gint, gint>>(get_size());
                gtk_window_resize(GTK_WINDOW(get_handle()), rw, rh);
                gtk_window_set_default_size(GTK_WINDOW(get_handle()), rw, rh);
                gtk_window_move(GTK_WINDOW(get_handle()), (gint)get_x(), (gint)get_y());
            }
        }
        draw_title();
        if (get_child())
        {
            draw_child();
        }
        gtk_widget_show_all(get_handle());
        draw_resizable();
    }

    void window::__parent_redraw()
    {
        if (get_handle())
            __draw({});
    }

    void window::draw_title()
    {
        gtk_window_set_title(GTK_WINDOW(get_handle()), m_title.data());
    }

    void window::draw_child()
    {
        get_child()->__draw(get_client_region());
        if (get_handle() != get_child()->get_handle())
        {
            g_list_free_unique_ptr list{ gtk_container_get_children(GTK_CONTAINER(get_handle())) };
            if (!list || list->data != get_child()->get_handle())
            {
                if (list) gtk_container_remove(GTK_CONTAINER(get_handle()), GTK_WIDGET(list->data));
                gtk_container_add(GTK_CONTAINER(get_handle()), get_child()->get_handle());
            }
        }
    }

    void window::draw_resizable()
    {
        gtk_window_set_resizable(GTK_WINDOW(get_handle()), m_resizable ? TRUE : FALSE);
    }

    void window::show()
    {
        __draw({});
    }

    rectangle window::get_client_region() const
    {
        gint width, height;
        gtk_window_get_size(GTK_WINDOW(get_handle()), &width, &height);
        return { 0, 0, (double)width, (double)height };
    }

    void window::on_destroy(GtkWidget* w, gpointer arg)
    {
        if (!(--application::current()->wnd_num)) gtk_main_quit();
    }

    gboolean window::on_configure_event(GtkWidget* widget, GdkEvent* event, gpointer data)
    {
        window* self = (window*)data;
        if (event->type == GDK_CONFIGURE && self->get_handle() && !self->m_resizing.exchange(true))
        {
            gint x, y;
            gtk_window_get_position(GTK_WINDOW(self->get_handle()), &x, &y);
            self->set_location({ (double)x, (double)y });
            gint width, height;
            gtk_window_get_size(GTK_WINDOW(self->get_handle()), &width, &height);
            self->set_size({ (double)width, (double)height });
            self->__draw({});
            self->m_resizing = false;
        }
        return FALSE;
    }
} // namespace xaml
