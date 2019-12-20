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

    optional<LRESULT> grid::wnd_proc(window_message const& msg)
    {
        for (auto c : m_children)
        {
            c->wnd_proc(msg);
        }
        return nullopt;
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
        rectangle real = region - get_margin();
        vector<double> columns = get_real_length(m_columns, real.width);
        vector<double> rows = get_real_length(m_rows, real.height);
        for (auto& c : m_children)
        {
            auto index = m_indecies[c];
            double subx = (index.column > 0 ? columns[index.column - 1] : 0) + real.x;
            double suby = (index.row > 0 ? rows[index.row - 1] : 0) + real.y;
            c->draw({ subx, suby, columns[index.column], rows[index.row] });
        }
    }
} // namespace xaml
