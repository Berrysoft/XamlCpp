#include <xaml/ui/grid.hpp>

using namespace std;

namespace xaml
{
    grid::grid() : multicontainer()
    {
    }

    grid::~grid() {}

    typename grid::native_handle_type grid::get_handle() const noexcept
    {
        return get_parent() ? get_parent()->get_handle() : nullptr;
    }

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
        double x = region.x + get_margin().left;
        double y = region.y + get_margin().top;
        double width = region.width - get_margin().left - get_margin().right;
        double height = region.height - get_margin().top - get_margin().bottom;
        vector<double> columns = get_real_length(m_columns, width);
        vector<double> rows = get_real_length(m_rows, height);
        for (auto pair : m_children)
        {
            auto index = m_indecies[pair.second];
            double subx = (index.column > 0 ? columns[index.column - 1] : 0) + x;
            double suby = (index.row > 0 ? rows[index.row - 1] : 0) + y;
            pair.second->draw({ subx, suby, columns[index.column], rows[index.row] });
        }
    }
} // namespace xaml
