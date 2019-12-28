#include <algorithm>
#include <internal/shared/grid.hpp>

using namespace std;

namespace xaml
{
    static double get_max_compact(size_t index, vector<shared_ptr<control>> const& children, bool vertical)
    {
        double result = 0;
        for (auto& c : children)
        {
            if ((vertical ? grid::get_row(c) : grid::get_column(c)) == index)
            {
                auto csize = c->__get_compact_size();
                auto len = vertical ? (csize.height + c->get_margin().top + c->get_margin().bottom) : (csize.width + c->get_margin().left + c->get_margin().right);
                result = (max)(result, len);
            }
        }
        return result;
    }

    vector<tuple<double, double>> get_real_length(vector<grid_length> const& lengths, vector<shared_ptr<control>> const& children, double total, bool vertical)
    {
        vector<tuple<double, double>> result(lengths.size());
        if (result.empty())
        {
            result.push_back(make_tuple(0, total));
            return result;
        }
        double total_star = 0;
        double total_remain = total;
        for (size_t i = 0; i < lengths.size(); i++)
        {
            switch (lengths[i].layout)
            {
            case grid_layout::abs:
                get<0>(result[i]) = lengths[i].value;
                total_remain -= lengths[i].value;
                break;
            case grid_layout::star:
                total_star += lengths[i].value;
                break;
            case grid_layout::compact:
            {
                get<0>(result[i]) = get_max_compact(i, children, vertical);
                total_remain -= get<0>(result[i]);
                break;
            }
            }
        }
        for (size_t i = 0; i < lengths.size(); i++)
        {
            if (lengths[i].layout == grid_layout::star)
            {
                get<0>(result[i]) = total_remain * lengths[i].value / total_star;
            }
            if (i)
            {
                get<1>(result[i]) = get<0>(result[i - 1]) + get<1>(result[i - 1]);
            }
        }
        return result;
    }

    unordered_map<shared_ptr<control>, grid_index> grid::m_indecies{};

    grid::grid() : multicontainer()
    {
    }

    grid::~grid() {}
} // namespace xaml
