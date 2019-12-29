#include <internal/shared/grid.hpp>
#include <xaml/ui/grid.hpp>

using namespace std;

namespace xaml
{
    optional<LRESULT> grid::__wnd_proc(window_message const& msg)
    {
        for (auto c : m_children)
        {
            c->__wnd_proc(msg);
        }
        return nullopt;
    }

    void grid::__draw(rectangle const& region)
    {
        bool new_draw = !get_handle();
        set_handle(get_parent()->get_handle());
        if (new_draw)
        {
            for (auto& c : m_children)
            {
                c->__draw(rectangle{ 0, 0, 0, 0 } + c->get_margin());
                c->__size_to_fit();
            }
        }
        rectangle real = region - get_margin();
        vector<tuple<double, double>> columns = get_real_length(m_columns, get_children(), real.width, false);
        vector<tuple<double, double>> rows = get_real_length(m_rows, get_children(), real.height, true);
        for (auto& c : m_children)
        {
            auto index = m_indecies[c];
            double subx = get<1>(columns[index.column]) + real.x;
            double suby = get<1>(rows[index.row]) + real.y;
            rectangle subrect = { subx, suby, get<0>(columns[index.column]), get<0>(rows[index.row]) };
            subrect = get_real_region(c, subrect);
            c->__draw(subrect);
        }
    }
} // namespace xaml
