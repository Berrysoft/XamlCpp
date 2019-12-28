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
        vector<tuple<double, double>> columns = get_real_length(m_columns, get_children(), real.width, false);
        vector<tuple<double, double>> rows = get_real_length(m_rows, get_children(), real.height, true);
        for (auto& c : m_children)
        {
            auto index = m_indecies[c];
            double subx = get<1>(columns[index.column]) + real.x;
            double suby = get<1>(rows[index.row]) + real.y;
            rectangle subrect = { subx, suby, get<0>(columns[index.column]), get<0>(rows[index.row]) };
            double cwidth = c->__get_compact_size().width + c->get_margin().left + c->get_margin().right;
            switch (c->get_halignment())
            {
            case halignment_t::left:
                subrect.width = cwidth;
                break;
            case halignment_t::center:
                subrect.x += (subrect.width - cwidth) / 2;
                subrect.width = cwidth;
                break;
            case halignment_t::right:
                subrect.x += subrect.width - cwidth;
                subrect.width = cwidth;
                break;
            }
            double cheight = c->__get_compact_size().height + c->get_margin().top + c->get_margin().bottom;
            switch (c->get_valignment())
            {
            case valignment_t::top:
                subrect.height = cheight;
                break;
            case valignment_t::center:
                subrect.y += (subrect.height - cheight) / 2;
                subrect.height = cheight;
                break;
            case valignment_t::bottom:
                subrect.y += subrect.height - cheight;
                subrect.height = cheight;
                break;
            }
            c->__draw(subrect);
        }
    }
} // namespace xaml
