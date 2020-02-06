#include <xaml/ui/controls/layout_base.hpp>

using namespace std;

namespace xaml
{
    void layout_base::__draw(rectangle const& region)
    {
        set_handle(get_parent()->get_handle());
        NSView* view = get_handle();
        __draw_impl(region, [this, view](shared_ptr<control> c, rectangle const& subrect) {
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
