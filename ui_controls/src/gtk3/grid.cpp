#include <xaml/ui/controls/grid.hpp>

using namespace std;

namespace xaml
{
    void grid::__draw(rectangle const& region)
    {
        bool new_draw = !get_handle();
        if (!get_handle())
        {
            set_handle(gtk_fixed_new());
            if (get_parent())
                gtk_container_add(GTK_CONTAINER(get_parent()->get_handle()), get_handle());
        }
        draw_impl(new_draw, region, [this](shared_ptr<control> c, rectangle const& subrect) {
            if (c->get_handle() && c->get_handle() != get_handle())
            {
                rectangle subreal = subrect - c->get_margin();
                if (!m_put_map[c])
                {
                    gtk_fixed_put(GTK_FIXED(get_handle()), c->get_handle(), (gint)subreal.x, (gint)subreal.y);
                    m_put_map[c] = true;
                }
                gtk_fixed_move(GTK_FIXED(get_handle()), c->get_handle(), (gint)subreal.x, (gint)subreal.y);
            }
        });
    }
} // namespace xaml
