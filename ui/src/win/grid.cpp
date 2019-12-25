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
        vector<double> columns = get_real_length(m_columns, real.width);
        vector<double> rows = get_real_length(m_rows, real.height);
        for (auto& c : m_children)
        {
            auto index = m_indecies[c];
            double subx = (index.column > 0 ? columns[index.column - 1] : 0) + real.x;
            double suby = (index.row > 0 ? rows[index.row - 1] : 0) + real.y;
            c->__draw({ subx, suby, columns[index.column], rows[index.row] });
        }
    }
} // namespace xaml
