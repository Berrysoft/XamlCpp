#include <shared/grid.hpp>
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
            rectangle subreal = subrect - c->get_margin();
            if (!m_put_map[c])
            {
                gtk_fixed_put(GTK_FIXED(get_handle()), c->get_handle(), (gint)subreal.x, (gint)subreal.y);
                m_put_map[c] = true;
            }
            gtk_fixed_move(GTK_FIXED(get_handle()), c->get_handle(), (gint)subreal.x, (gint)subreal.y);
        }
    }
} // namespace xaml
