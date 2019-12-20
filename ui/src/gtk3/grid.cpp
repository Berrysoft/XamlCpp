#include <vector>
#include <xaml/ui/grid.hpp>

using namespace std;

namespace xaml
{
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
        }
        double x = region.x + get_margin().left;
        double y = region.y + get_margin().top;
        double width = region.width - get_margin().left - get_margin().right;
        double height = region.height - get_margin().top - get_margin().bottom;
        vector<double> columns = get_real_length(m_columns, width);
        vector<double> rows = get_real_length(m_rows, height);
        for (auto c : m_children)
        {
            auto index = m_indecies[c];
            double subx = (index.column > 0 ? columns[index.column - 1] : 0) + x;
            double suby = (index.row > 0 ? rows[index.row - 1] : 0) + y;
            c->draw({ subx, suby, columns[index.column], rows[index.row] });
            if (new_draw)
                gtk_fixed_put(GTK_FIXED(get_handle()), c->get_handle(), subx + c->get_margin().left, suby + c->get_margin().top);
            else
                gtk_fixed_move(GTK_FIXED(get_handle()), c->get_handle(), subx + c->get_margin().left, suby + c->get_margin().top);
        }
    }
} // namespace xaml
