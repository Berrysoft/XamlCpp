#include <shared/control.hpp>
#include <xaml/ui/control.h>
#include <xaml/ui/drawing_conv.hpp>

using namespace std;

xaml_result xaml_control_internal::set_rect(xaml_rectangle const& region) noexcept
{
    xaml_rectangle real = region - m_margin;
    XAML_RETURN_IF_FAILED(set_size_noevent({ real.width, real.height }));
    XAML_RETURN_IF_FAILED(draw_size());
    return XAML_S_OK;
}

xaml_result xaml_control_internal::size_to_fit() noexcept
{
    int width, height;
    gtk_widget_get_preferred_width(m_handle, NULL, &width);
    gtk_widget_get_preferred_height(m_handle, NULL, &height);
    return set_size_noevent({ (double)width, (double)height });
}

xaml_result xaml_control_internal::draw_size() noexcept
{
    auto [rw, rh] = xaml_to_native<tuple<gint, gint>>(m_size);
    gtk_widget_set_size_request(m_handle, rw, rh);
    return XAML_S_OK;
}

xaml_result xaml_control_internal::draw_visible() noexcept
{
    gtk_widget_set_visible(m_handle, m_is_visible);
    return XAML_S_OK;
}

gboolean xaml_control_internal::on_button_event(GtkWidget*, GdkEventButton* event, xaml_control_internal* self) noexcept
{
    xaml_mouse_button button;
    switch (event->button)
    {
    case 1:
        button = xaml_mouse_button_left;
        break;
    case 2:
        button = xaml_mouse_button_middle;
        break;
    case 3:
        button = xaml_mouse_button_right;
        break;
    default:
        button = xaml_mouse_button_other;
        break;
    }
    switch (event->type)
    {
    case GDK_BUTTON_PRESS:
        XAML_ASSERT_SUCCEEDED(self->on_mouse_down(self->m_outer_this, button));
        break;
    case GDK_BUTTON_RELEASE:
        XAML_ASSERT_SUCCEEDED(self->on_mouse_up(self->m_outer_this, button));
        break;
    }
    return TRUE;
}

gboolean xaml_control_internal::on_button_motion(GtkWidget*, GdkEventMotion* event, xaml_control_internal* self) noexcept
{
    XAML_ASSERT_SUCCEEDED(self->on_mouse_move(self->m_outer_this, xaml_point{ event->x, event->y }));
    return TRUE;
}
