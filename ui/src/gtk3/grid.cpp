#include <internal/shared/grid.hpp>
#include <xaml/ui/grid.hpp>

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
        rectangle real = region - get_margin();
        vector<tuple<double, double>> columns = get_real_length(m_columns, real.width);
        vector<tuple<double, double>> rows = get_real_length(m_rows, real.height);
        for (auto& c : m_children)
        {
            auto index = m_indecies[c];
            double subx = get<1>(columns[index.column]) + real.x;
            double suby = get<1>(rows[index.row]) + real.y;
            rectangle subregion = { subx, suby, get<0>(columns[index.column]), get<0>(rows[index.row]) };
            c->__draw(subregion);
            rectangle subreal = subregion - c->get_margin();
            if (new_draw)
                gtk_fixed_put(GTK_FIXED(get_handle()), c->get_handle(), subreal.x, subreal.y);
            else
                gtk_fixed_move(GTK_FIXED(get_handle()), c->get_handle(), subreal.x, subreal.y);
        }
    }
} // namespace xaml
