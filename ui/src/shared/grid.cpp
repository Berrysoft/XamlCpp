#include <algorithm>
#include <shared/grid.hpp>

using namespace std;

namespace xaml
{
#ifndef XAML_UI_WINRT
    static double get_max_compact(size_t index, vector<shared_ptr<control>> const& children, bool vertical)
    {
        double result = 0;
        for (auto& c : children)
        {
            if ((vertical ? grid::get_row(c) : grid::get_column(c)) == index)
            {
                auto csize = c->get_size();
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
            result.push_back(make_tuple(total, 0));
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

    rectangle get_real_region(shared_ptr<control> c, rectangle max_region)
    {
        double cwidth = c->get_width() + c->get_margin().left + c->get_margin().right;
        switch (c->get_halignment())
        {
        case halignment_t::left:
            max_region.width = cwidth;
            break;
        case halignment_t::center:
            max_region.x += (max_region.width - cwidth) / 2;
            max_region.width = cwidth;
            break;
        case halignment_t::right:
            max_region.x += max_region.width - cwidth;
            max_region.width = cwidth;
            break;
        default:
            break;
        }
        double cheight = c->get_height() + c->get_margin().top + c->get_margin().bottom;
        switch (c->get_valignment())
        {
        case valignment_t::top:
            max_region.height = cheight;
            break;
        case valignment_t::center:
            max_region.y += (max_region.height - cheight) / 2;
            max_region.height = cheight;
            break;
        case valignment_t::bottom:
            max_region.y += max_region.height - cheight;
            max_region.height = cheight;
            break;
        default:
            break;
        }
        return max_region;
    }

#endif // !XAML_UI_WINRT

    XAML_API unordered_map<shared_ptr<control>, grid_index> grid::m_indecies{};

    XAML_API grid::grid() : multicontainer()
    {
    }

    XAML_API grid::~grid() {}
} // namespace xaml
