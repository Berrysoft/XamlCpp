#include <gtk3/resources.hpp>
#include <shared/atomic_guard.hpp>
#include <shared/window.hpp>
#include <xaml/ui/application.h>
#include <xaml/ui/gtk3/xamlfixed.h>
#include <xaml/ui/menu_bar.h>
#include <xaml/ui/native_drawing.hpp>
#include <xaml/ui/window.h>

using namespace std;

xaml_window_internal::~xaml_window_internal()
{
    if (m_handle && !gtk_widget_in_destruction(m_window_handle))
    {
        gtk_widget_destroy(m_window_handle);
    }
}

xaml_result xaml_window_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        m_window_handle = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        m_vbox_handle = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        m_handle = xaml_fixed_new();
        gtk_container_add(GTK_CONTAINER(m_window_handle), m_vbox_handle);
        gtk_box_pack_end(GTK_BOX(m_vbox_handle), m_handle, TRUE, TRUE, 0);
        xaml_ptr<xaml_application> app;
        XAML_RETURN_IF_FAILED(xaml_application_current(&app));
        XAML_RETURN_IF_FAILED(app->window_added(static_cast<xaml_window*>(m_outer_this)));
        g_signal_connect(G_OBJECT(m_window_handle), "destroy", G_CALLBACK(xaml_window_internal::on_destroy), this);
        g_signal_connect(G_OBJECT(m_window_handle), "delete-event", G_CALLBACK(xaml_window_internal::on_delete_event), this);
        g_signal_connect(G_OBJECT(m_window_handle), "configure-event", G_CALLBACK(xaml_window_internal::on_configure_event), this);
        XAML_RETURN_IF_FAILED(draw_title());
        XAML_RETURN_IF_FAILED(draw_resizable());
    }
    XAML_RETURN_IF_FAILED(draw_size());
    XAML_RETURN_IF_FAILED(draw_child());
    XAML_RETURN_IF_FAILED(draw_menu_bar());
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_size() noexcept
{
    xaml_atomic_guard guard{ m_resizing };
    if (!guard.test_and_set())
    {
        auto [rw, rh] = xaml_to_native<tuple<gint, gint>>(m_size);
        gtk_window_resize(GTK_WINDOW(m_window_handle), rw, rh);
        gtk_window_set_default_size(GTK_WINDOW(m_window_handle), rw, rh);
        gtk_window_move(GTK_WINDOW(m_window_handle), (gint)m_location.x, (gint)m_location.y);
    }
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_title() noexcept
{
    xaml_char_t const* title;
    XAML_RETURN_IF_FAILED(m_title->get_data(&title));
    gtk_window_set_title(GTK_WINDOW(m_window_handle), title);
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_child() noexcept
{
    if (m_child)
    {
        xaml_rectangle region;
        XAML_RETURN_IF_FAILED(get_client_region(&region));
        return m_child->draw(region);
    }
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_resizable() noexcept
{
    gtk_window_set_resizable(GTK_WINDOW(m_window_handle), m_is_resizable);
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_menu_bar() noexcept
{
    //if (m_menu_bar)
    //{
    //    m_menu_bar->set_parent_window(shared_from_this<window>());
    //    m_menu_bar->__draw({});
    //    g_list_free_unique_ptr list{ gtk_container_get_children(GTK_CONTAINER(get_window()->vbox)) };
    //    if (!g_list_find(list.get(), m_menu_bar->get_handle()->handle))
    //    {
    //        gtk_box_pack_start(GTK_BOX(get_window()->vbox), m_menu_bar->get_handle()->handle, FALSE, FALSE, 0);
    //    }
    //}
    return XAML_S_OK;
}

xaml_result xaml_window_internal::show() noexcept
{
    XAML_RETURN_IF_FAILED(draw({}));
    gtk_widget_show_all(m_window_handle);
    return set_is_visible(true);
}

xaml_result xaml_window_internal::close() noexcept
{
    gtk_window_close(GTK_WINDOW(m_window_handle));
    return XAML_S_OK;
}

xaml_result xaml_window_internal::hide() noexcept
{
    gtk_widget_hide(m_window_handle);
    return set_is_visible(false);
}

xaml_result xaml_window_internal::get_client_region(xaml_rectangle* pregion) noexcept
{
    gint width, height;
    gtk_window_get_size(GTK_WINDOW(m_window_handle), &width, &height);
    //if (m_menu_bar && get_menu_bar()->get_handle())
    //{
    //    gint mheight = gtk_widget_get_allocated_height(get_menu_bar()->get_handle()->handle);
    //    height -= mheight;
    //}
    *pregion = { 0, 0, (double)width, (double)height };
    return XAML_S_OK;
}

static xaml_result xaml_window_internal_on_destroy(GtkWidget* widget, xaml_window_internal* self) noexcept
{
    xaml_ptr<xaml_application> app;
    XAML_RETURN_IF_FAILED(xaml_application_current(&app));
    XAML_RETURN_IF_FAILED(app->window_removed(static_cast<xaml_window*>(self->m_outer_this)));
    return self->set_handle(nullptr);
}

void xaml_window_internal::on_destroy(GtkWidget* widget, xaml_window_internal* self) noexcept
{
    XAML_ASSERT_SUCCESS(xaml_window_internal_on_destroy(widget, self));
}

static xaml_result xaml_window_internal_on_delete_event(GtkWidget* widget, GdkEvent* event, xaml_window_internal* self, gboolean* pres) noexcept
{
    auto handled = xaml_box_value(false);
    XAML_RETURN_IF_FAILED(self->on_closing(self->m_outer_this, handled));
    bool value;
    XAML_RETURN_IF_FAILED(xaml_unbox_value(handled.get(), value));
    *pres = value;
    return XAML_S_OK;
}

gboolean xaml_window_internal::on_delete_event(GtkWidget* widget, GdkEvent* event, xaml_window_internal* self) noexcept
{
    gboolean res;
    XAML_ASSERT_SUCCESS(xaml_window_internal_on_delete_event(widget, event, self, &res));
    return res;
}

static xaml_result xaml_window_internal_on_configure_event(GtkWidget* widget, GdkEvent* event, xaml_window_internal* self, gboolean* pres) noexcept
{
    if (event->type == GDK_CONFIGURE && self->m_handle)
    {
        xaml_atomic_guard guard{ self->m_resizing };
        if (!guard.test_and_set())
        {
            gint x, y;
            gtk_window_get_position(GTK_WINDOW(self->m_window_handle), &x, &y);
            XAML_RETURN_IF_FAILED(self->set_location({ (double)x, (double)y }));
            gint width, height;
            gtk_window_get_size(GTK_WINDOW(self->m_window_handle), &width, &height);
            XAML_RETURN_IF_FAILED(self->set_size({ (double)width, (double)height }));
            XAML_RETURN_IF_FAILED(self->draw({}));
        }
    }
    *pres = FALSE;
    return XAML_S_OK;
}

gboolean xaml_window_internal::on_configure_event(GtkWidget* widget, GdkEvent* event, xaml_window_internal* self) noexcept
{
    gboolean res;
    XAML_ASSERT_SUCCESS(xaml_window_internal_on_configure_event(widget, event, self, &res));
    return res;
}

xaml_result xaml_window_internal::get_dpi(double* pvalue) noexcept
{
    GdkScreen* screen = gtk_window_get_screen(GTK_WINDOW(m_window_handle));
    gdouble res = gdk_screen_get_resolution(screen);
    if (res < 0)
        *pvalue = 96.0;
    else
        *pvalue = res;
    return XAML_S_OK;
}
