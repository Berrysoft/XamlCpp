#include <xaml/ui/controls/layout_base.hpp>

using namespace std;

namespace xaml
{
    void layout_base::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            set_handle(gtk_fixed_new());
        }
        rectangle real = region - get_margin();
        __draw_impl(region, [this, &real](shared_ptr<control> c, rectangle const& subrect) {
            if (c->get_handle() && c->get_handle() != get_handle())
            {
                rectangle subreal = subrect - c->get_margin();
                if (!m_put_map[c])
                {
                    gtk_fixed_put(GTK_FIXED(get_handle()), c->get_handle(), (gint)(subreal.x - real.x), (gint)(subreal.y - real.y));
                    m_put_map[c] = true;
                }
                gtk_fixed_move(GTK_FIXED(get_handle()), c->get_handle(), (gint)(subreal.x - real.x), (gint)(subreal.y - real.y));
            }
        });
    }
} // namespace xaml
