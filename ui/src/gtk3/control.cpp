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
