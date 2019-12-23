#include <vector>
#include <xaml/ui/grid.hpp>

using namespace std;

namespace xaml
{
    unordered_map<shared_ptr<control>, grid_index> grid::m_indecies{};

    grid::grid() : multicontainer()
    {
    }

    grid::~grid() {}

    static vector<double> get_real_length(vector<grid_length> const& lengths, double total)
    {
        vector<double> result(lengths.size());
        if (result.empty())
        {
            result.push_back(total);
            return result;
        }
        double total_star = 0;
        double total_remain = total;
        for (size_t i = 0; i < lengths.size(); i++)
        {
            switch (lengths[i].layout)
            {
            case grid_layout::abs:
                result[i] = lengths[i].value;
                total_remain -= lengths[i].value;
                break;
            case grid_layout::star:
                total_star += lengths[i].value;
                break;
            }
        }
        for (size_t i = 0; i < lengths.size(); i++)
        {
            if (lengths[i].layout == grid_layout::star)
            {
                result[i] = total_remain * lengths[i].value / total_star;
            }
        }
        return result;
    }

    void grid::draw(rectangle const& region)
    {
        bool new_draw = !get_handle();
        if (!get_handle())
        {
            set_handle(gtk_fixed_new());
            if (get_parent())
                gtk_container_add(GTK_CONTAINER(get_parent()->get_handle()), get_handle());
        }
        rectangle real = region - get_margin();
        vector<double> columns = get_real_length(m_columns, real.width);
        vector<double> rows = get_real_length(m_rows, real.height);
        for (auto& c : m_children)
        {
            auto index = m_indecies[c];
            double subx = (index.column > 0 ? columns[index.column - 1] : 0) + real.x;
            double suby = (index.row > 0 ? rows[index.row - 1] : 0) + real.y;
            rectangle subregion = { subx, suby, columns[index.column], rows[index.row] };
            c->draw(subregion);
            rectangle subreal = subregion - c->get_margin();
            if (new_draw)
                gtk_fixed_put(GTK_FIXED(get_handle()), c->get_handle(), subreal.x, subreal.y);
            else
                gtk_fixed_move(GTK_FIXED(get_handle()), c->get_handle(), subreal.x, subreal.y);
        }
    }
} // namespace xaml
