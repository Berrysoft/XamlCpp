#include <algorithm>
#include <numeric>
#include <xaml/ui/controls/grid.hpp>

using namespace std;

namespace xaml
{
    static double get_max_compact(size_t index, array_view<std::shared_ptr<control>> children, bool vertical)
    {
        double result = 0;
        for (auto& c : children)
        {
            if (((vertical ? grid::get_row(c) : grid::get_column(c)) == index) && ((vertical ? grid::get_row_span(c) : grid::get_column_span(c)) <= 1))
            {
                auto csize = c->get_size();
                auto len = vertical ? (csize.height + c->get_margin().top + c->get_margin().bottom) : (csize.width + c->get_margin().left + c->get_margin().right);
                result = (max)(result, len);
            }
        }
        return result;
    }

    static vector<tuple<double, double>> get_real_length(array_view<grid_length> lengths, array_view<std::shared_ptr<control>> children, double total, bool vertical)
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

    static rectangle get_real_region(shared_ptr<control> c, rectangle max_region)
    {
        double cwidth = c->get_width() + c->get_margin().left + c->get_margin().right;
        cwidth = (min)(cwidth, max_region.width);
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
        cheight = (min)(cheight, max_region.height);
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

    unordered_map<shared_ptr<control>, grid_index> grid::m_indecies{};

    grid::grid() : multicontainer()
    {
    }

    grid::~grid() {}

    void grid::draw_impl(bool new_draw, rectangle const& region, function<void(std::shared_ptr<control>, rectangle const&)> func)
    {
        for (auto& c : m_children)
        {
            if (new_draw) c->__draw(rectangle{ 0, 0, 0, 0 } + c->get_margin());
            c->__size_to_fit();
        }
        rectangle real = region - get_margin();
        vector<tuple<double, double>> columns = get_real_length(m_columns, get_children(), real.width, false);
        vector<tuple<double, double>> rows = get_real_length(m_rows, get_children(), real.height, true);
        auto real_length_plus = [](double lhs, tuple<double, double> const& rhs) { return lhs + get<0>(rhs); };
        for (auto& c : m_children)
        {
            auto index = m_indecies[c];
            double subx = get<1>(columns[(min)(index.column, columns.size() - 1)]) + real.x;
            double suby = get<1>(rows[(min)(index.row, rows.size() - 1)]) + real.y;
            double subw = accumulate(columns.begin() + (min)(index.column, columns.size() - 1), columns.begin() + (min)(index.column + (max<size_t>)(index.column_span, 1), columns.size()), 0.0, real_length_plus);
            double subh = accumulate(rows.begin() + (min)(index.row, rows.size() - 1), rows.begin() + (min)(index.row + (max<size_t>)(index.row_span, 1), rows.size()), 0.0, real_length_plus);
            rectangle subrect = { subx, suby, subw, subh };
            subrect = get_real_region(c, subrect);
            c->__draw(subrect);
            if (func) func(c, subrect);
        }
    }
} // namespace xaml
