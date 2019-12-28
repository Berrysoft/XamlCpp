#include <internal/shared/grid.hpp>
#include <xaml/ui/grid.hpp>

using namespace std;

namespace xaml
{
    void grid::__draw(rectangle const& region)
    {
        bool new_draw = !get_handle();
        set_handle(get_parent()->get_handle());
        if (new_draw)
        {
            for (auto& c : m_children)
            {
                c->__draw(rectangle{ 0, 0, 1, 1 } + c->get_margin());
            }
        }
        rectangle real = region - get_margin();
        vector<tuple<double, double>> columns = get_real_length(m_columns, get_children(), real.width, false);
        vector<tuple<double, double>> rows = get_real_length(m_rows, get_children(), real.height, true);
        NSView* view = get_handle();
        for (auto& c : m_children)
        {
            auto index = m_indecies[c];
            double subx = get<1>(columns[index.column]) + real.x;
            double suby = get<1>(rows[index.row]) + real.y;
            c->__draw({ subx, suby, get<0>(columns[index.column]), get<0>(rows[index.row]) });
            if (c->get_handle() && c->get_handle() != get_handle())
            {
                if (!m_put_map[c])
                {
                    [view addSubview:(NSView*)c->get_handle()];
                    m_put_map[c] = true;
                }
            }
        }
    }
}
