#include <gtk3/resources.hpp>
#include <shared/atomic_guard.hpp>
#include <xaml/ui/application.hpp>
#include <xaml/ui/gtk3/xamlfixed.h>
#include <xaml/ui/menu_bar.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_drawing.hpp>
#include <xaml/ui/native_window.hpp>
#include <xaml/ui/window.hpp>

using namespace std;

namespace xaml
{
    window::~window()
    {
        if (get_handle() && !gtk_widget_in_destruction(get_window()->window))
        {
            gtk_widget_destroy(get_window()->window);
        }
    }

    void window::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            auto w = make_shared<native_window>();
            w->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
            w->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            set_window(w);
            auto h = make_shared<native_control>();
            h->handle = xaml_fixed_new();
            set_handle(h);
            gtk_container_add(GTK_CONTAINER(get_window()->window), get_window()->vbox);
            gtk_box_pack_end(GTK_BOX(get_window()->vbox), h->handle, TRUE, TRUE, 0);
            application::current()->window_added(static_pointer_cast<window>(shared_from_this()));
            g_signal_connect(G_OBJECT(get_window()->window), "destroy", G_CALLBACK(window::on_destroy), this);
            g_signal_connect(G_OBJECT(get_window()->window), "delete-event", G_CALLBACK(window::on_delete_event), this);
            g_signal_connect(G_OBJECT(get_window()->window), "configure-event", G_CALLBACK(window::on_configure_event), this);
            draw_title();
            draw_resizable();
        }
        draw_size();
        draw_child();
        draw_menu_bar();
    }

    void window::__parent_redraw()
    {
        if (get_handle())
            __draw({});
    }

    void window::draw_size()
    {
        atomic_guard guard{ m_resizing };
        if (!guard.test_and_set())
        {
            auto [rw, rh] = to_native<tuple<gint, gint>>(get_size());
            gtk_window_resize(GTK_WINDOW(get_window()->window), rw, rh);
            gtk_window_set_default_size(GTK_WINDOW(get_window()->window), rw, rh);
            gtk_window_move(GTK_WINDOW(get_window()->window), (gint)get_x(), (gint)get_y());
        }
    }

    void window::draw_title()
    {
        gtk_window_set_title(GTK_WINDOW(get_window()->window), m_title.data());
    }

    void window::draw_child()
    {
        if (get_child())
            get_child()->__draw(get_client_region());
    }

    void window::draw_resizable()
    {
        gtk_window_set_resizable(GTK_WINDOW(get_window()->window), m_resizable ? TRUE : FALSE);
    }

    void window::draw_menu_bar()
    {
        if (m_menu_bar)
        {
            m_menu_bar->set_parent_window(static_pointer_cast<window>(shared_from_this()));
            m_menu_bar->__draw({});
            g_list_free_unique_ptr list{ gtk_container_get_children(GTK_CONTAINER(get_window()->vbox)) };
            if (!g_list_find(list.get(), m_menu_bar->get_handle()->handle))
            {
                gtk_box_pack_start(GTK_BOX(get_window()->vbox), m_menu_bar->get_handle()->handle, FALSE, FALSE, 0);
            }
        }
    }

    void window::show()
    {
        __draw({});
        gtk_widget_show_all(get_window()->window);
        set_is_visible(true);
    }

    void window::close()
    {
        gtk_window_close(GTK_WINDOW(get_window()->window));
    }

    void window::hide()
    {
        gtk_widget_hide(get_window()->window);
        set_is_visible(false);
    }

    rectangle window::get_client_region() const
    {
        gint width, height;
        gtk_window_get_size(GTK_WINDOW(get_window()->window), &width, &height);
        if (get_menu_bar() && get_menu_bar()->get_handle())
        {
            gint mheight = gtk_widget_get_allocated_height(get_menu_bar()->get_handle()->handle);
            height -= mheight;
        }
        return { 0, 0, (double)width, (double)height };
    }

    void window::on_destroy(void* widget, void* data)
    {
        window* self = (window*)data;
        application::current()->window_removed(static_pointer_cast<window>(self->shared_from_this()));
        self->set_handle(nullptr);
    }

    int window::on_delete_event(void* widget, void* event, void* data)
    {
        window* self = (window*)data;
        auto handled = box_value(false);
        self->m_closing(static_pointer_cast<window>(self->shared_from_this()), handled);
        return *handled;
    }

    int window::on_configure_event(void* widget, void* event, void* data)
    {
        window* self = (window*)data;
        if (((GdkEvent*)event)->type == GDK_CONFIGURE && self->get_handle())
        {
            atomic_guard guard{ self->m_resizing };
            if (!guard.test_and_set())
            {
                gint x, y;
                gtk_window_get_position(GTK_WINDOW(self->get_window()->window), &x, &y);
                self->set_location({ (double)x, (double)y });
                gint width, height;
                gtk_window_get_size(GTK_WINDOW(self->get_window()->window), &width, &height);
                self->set_size({ (double)width, (double)height });
                self->__draw({});
            }
        }
        return FALSE;
    }

    double window::get_dpi() const
    {
        GdkScreen* screen = gtk_window_get_screen(GTK_WINDOW(get_window()->window));
        gdouble res = gdk_screen_get_resolution(screen);
        if (res < 0)
            return 96.0;
        else
            return res;
    }
} // namespace xaml
