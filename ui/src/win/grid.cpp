#include <shared/grid.hpp>
#include <xaml/ui/grid.hpp>

using namespace std;

namespace xaml
{
    optional<LRESULT> grid::__wnd_proc(window_message const& msg)
    {
        optional<LRESULT> result = nullopt;
        for (auto c : m_children)
        {
            auto r = c->__wnd_proc(msg);
            if (r) result = r;
        }
        return result;
    }

    void grid::__draw(rectangle const& region)
    {
        bool new_draw = !get_handle();
        set_handle(get_parent()->get_handle());
        SendMessage(get_handle(), WM_SETREDRAW, FALSE, 0);
        for (auto& c : m_children)
        {
            if (new_draw) c->__draw(rectangle{ 0, 0, 0, 0 } + c->get_margin());
            c->__size_to_fit();
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
        SendMessage(get_handle(), WM_SETREDRAW, TRUE, 0);
    }
} // namespace xaml
