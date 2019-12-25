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
        set_handle(get_parent()->get_handle());
        rectangle real = region - get_margin();
        vector<tuple<double, double>> columns = get_real_length(m_columns, real.width);
        vector<tuple<double, double>> rows = get_real_length(m_rows, real.height);
        for (auto& c : m_children)
        {
            auto index = m_indecies[c];
            double subx = get<1>(columns[index.column]) + real.x;
            double suby = get<1>(rows[index.row]) + real.y;
            c->__draw({ subx, suby, get<0>(columns[index.column]), get<0>(rows[index.row]) });
        }
    }
} // namespace xaml
