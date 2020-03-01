#include <gtk3/xamlfixed.h>
#include <xaml/ui/controls/layout_base.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_drawing.hpp>

using namespace std;

namespace xaml
{
    void layout_base::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            auto h = make_shared<native_control>();
            h->handle = xaml_fixed_new();
            set_handle(h);
        }
        rectangle real = region - get_margin();
        __draw_impl(region, [this, &real](shared_ptr<control> c, rectangle const& subrect) {
            if (c->get_handle() && c->get_handle() != get_handle())
            {
                rectangle subreal = subrect - c->get_margin();
                if (!m_put_map[c])
                {
                    gtk_container_add(GTK_CONTAINER(get_handle()->handle), c->get_handle()->handle);
                    m_put_map[c] = true;
                }
                subreal.x -= real.x;
                subreal.y -= real.y;
                GtkAllocation alloc = to_native<GtkAllocation>(subreal);
                xaml_fixed_child_size_allocate(XAML_FIXED(get_handle()->handle), c->get_handle()->handle, &alloc);
            }
        });
    }
} // namespace xaml
