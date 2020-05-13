#include <shared/layout_base.hpp>
#include <xaml/ui/controls/layout_base.h>
#include <xaml/ui/gtk3/xamlfixed.h>
#include <xaml/ui/native_drawing.hpp>

using namespace std;

xaml_result xaml_layout_base_internal::draw(xaml_rectangle const& region) noexcept
{
    xaml_ptr<xaml_gtk3_control> native_parent;
    XAML_RETURN_IF_FAILED(m_parent->query(&native_parent));
    XAML_RETURN_IF_FAILED(native_parent->get_handle(&m_handle));
    return draw_impl(region, [this](xaml_control* c, xaml_rectangle const& subrect) -> xaml_result {
        xaml_ptr<xaml_gtk3_control> native_control;
        if (XAML_SUCCEEDED(c->query(&native_control)))
        {
            GtkWidget* handle;
            XAML_RETURN_IF_FAILED(native_control->get_handle(&handle));
            if (handle != m_handle)
            {
                xaml_margin margin;
                XAML_RETURN_IF_FAILED(c->get_margin(&margin));
                xaml_rectangle subreal = subrect - margin;
                if (!m_put_map[c])
                {
                    gtk_container_add(GTK_CONTAINER(m_handle), handle);
                    m_put_map[c] = true;
                }
                GtkAllocation alloc = xaml_to_native<GtkAllocation>(subreal);
                xaml_fixed_child_size_allocate(XAML_FIXED(m_handle), handle, &alloc);
            }
        }
        return XAML_S_OK;
    });
}
