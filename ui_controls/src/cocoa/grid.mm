#include <xaml/ui/controls/grid.hpp>

using namespace std;

namespace xaml
{
    void grid::__draw(rectangle const& region)
    {
        bool new_draw = !get_handle();
        set_handle(get_parent()->get_handle());
        NSView* view = get_handle();
        draw_impl(new_draw, region, [this, view](shared_ptr<control> c, rectangle const& subrect) {
            if (c->get_handle() && c->get_handle() != get_handle())
            {
                if (!m_put_map[c])
                {
                    [view addSubview:(NSView*)c->get_handle()];
                    m_put_map[c] = true;
                }
            }
        });
    }
}
