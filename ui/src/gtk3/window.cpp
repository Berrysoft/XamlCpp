#include <gtk3/resources.hpp>
#include <shared/atomic_guard.hpp>
#include <xaml/ui/application.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_drawing.hpp>
#include <xaml/ui/window.hpp>

using namespace std;

namespace xaml
{
    window::~window()
    {
        if (get_handle() && !gtk_widget_in_destruction(get_handle()->handle))
        {
            close();
            gtk_widget_destroy(get_handle()->handle);
        }
    }

    void window::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            auto h = make_shared<native_control>();
            h->handle = gtk_window_new(GTK_WINDOW_TOPLEVEL);
            set_handle(h);
            application::current()->window_added(static_pointer_cast<window>(shared_from_this()));
            g_signal_connect(G_OBJECT(get_handle()->handle), "destroy", G_CALLBACK(window::on_destroy), this);
            g_signal_connect(G_OBJECT(get_handle()->handle), "configure-event", G_CALLBACK(window::on_configure_event), this);
            draw_title();
            draw_resizable();
        }
        draw_size();
        draw_child();
        gtk_widget_show_all(get_handle()->handle);
    }

    void window::__parent_redraw()
    {
        if (get_handle())
            __draw({});
    }

    void window::draw_size()
    {
        atomic_guard guard{ m_resizing };
        if (!guard.exchange(true))
        {
            auto [rw, rh] = to_native<tuple<gint, gint>>(get_size());
            gtk_window_resize(GTK_WINDOW(get_handle()->handle), rw, rh);
            gtk_window_set_default_size(GTK_WINDOW(get_handle()->handle), rw, rh);
            gtk_window_move(GTK_WINDOW(get_handle()->handle), (gint)get_x(), (gint)get_y());
        }
    }

    void window::draw_title()
    {
        gtk_window_set_title(GTK_WINDOW(get_handle()->handle), m_title.data());
    }

    void window::draw_child()
    {
        if (get_child())
        {
            get_child()->__draw(get_client_region());
            if (get_handle() != get_child()->get_handle())
            {
                g_list_free_unique_ptr list{ gtk_container_get_children(GTK_CONTAINER(get_handle()->handle)) };
                if (!list || list->data != get_child()->get_handle()->handle)
                {
                    if (list) gtk_container_remove(GTK_CONTAINER(get_handle()->handle), GTK_WIDGET(list->data));
                    gtk_container_add(GTK_CONTAINER(get_handle()->handle), get_child()->get_handle()->handle);
                }
            }
        }
    }

    void window::draw_resizable()
    {
        gtk_window_set_resizable(GTK_WINDOW(get_handle()->handle), m_resizable ? TRUE : FALSE);
    }

    void window::show()
    {
        __draw({});
    }

    void window::close()
    {
        gtk_window_close(GTK_WINDOW(get_handle()->handle));
    }

    void window::hide()
    {
        gtk_widget_hide(get_handle()->handle);
    }

    rectangle window::get_client_region() const
    {
        gint width, height;
        gtk_window_get_size(GTK_WINDOW(get_handle()->handle), &width, &height);
        return { 0, 0, (double)width, (double)height };
    }

    void window::on_destroy(void* w, void* arg)
    {
        window* self = (window*)arg;
        application::current()->window_removed(static_pointer_cast<window>(self->shared_from_this()));
        self->set_handle(nullptr);
    }

    int window::on_configure_event(void* widget, void* event, void* data)
    {
        window* self = (window*)data;
        if (((GdkEvent*)event)->type == GDK_CONFIGURE && self->get_handle())
        {
            atomic_guard guard{ self->m_resizing };
            if (!guard.exchange(true))
            {
                gint x, y;
                gtk_window_get_position(GTK_WINDOW(self->get_handle()->handle), &x, &y);
                self->set_location({ (double)x, (double)y });
                gint width, height;
                gtk_window_get_size(GTK_WINDOW(self->get_handle()->handle), &width, &height);
                self->set_size({ (double)width, (double)height });
                self->__draw({});
            }
        }
        return FALSE;
    }

    double window::get_dpi() const
    {
        GdkScreen* screen = gtk_window_get_screen(GTK_WINDOW(get_handle()->handle));
        gdouble res = gdk_screen_get_resolution(screen);
        if (res < 0)
            return 96.0;
        else
            return res;
    }
} // namespace xaml
