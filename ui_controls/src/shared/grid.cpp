#include <algorithm>
#include <numeric>
#include <shared/grid.hpp>
#include <xaml/ui/controls/grid.h>

using namespace std;

static xaml_result get_max_compact(int32_t index, xaml_ptr<xaml_vector> const& children, bool vertical, double* presult) noexcept
{
    double result = 0;
    XAML_FOREACH_START(c, children);
    {
        xaml_ptr<xaml_control> cc;
        XAML_RETURN_IF_FAILED(c->query(&cc));
        int32_t crow, ccol, crows, ccols;
        XAML_RETURN_IF_FAILED(xaml_grid_get_column(cc.get(), &ccol));
        XAML_RETURN_IF_FAILED(xaml_grid_get_row(cc.get(), &crow));
        XAML_RETURN_IF_FAILED(xaml_grid_get_column_span(cc.get(), &ccols));
        XAML_RETURN_IF_FAILED(xaml_grid_get_row_span(cc.get(), &crows));
        if (((vertical ? crow : ccol) == index) && ((vertical ? crows : ccols) <= 1))
        {
            xaml_size csize;
            XAML_RETURN_IF_FAILED(cc->get_size(&csize));
            xaml_margin cmargin;
            XAML_RETURN_IF_FAILED(cc->get_margin(&cmargin));
            auto len = vertical ? (csize.height + cmargin.top + cmargin.bottom) : (csize.width + cmargin.left + cmargin.right);
            result = (max)(result, len);
        }
    }
    XAML_FOREACH_END();
    *presult = result;
    return XAML_S_OK;
}

static xaml_result get_real_length(xaml_ptr<xaml_vector> const& lengths, xaml_ptr<xaml_vector> const& children, double total, bool vertical, vector<tuple<double, double>>* presult) noexcept
{
    int32_t size;
    XAML_RETURN_IF_FAILED(lengths->get_size(&size));
    vector<tuple<double, double>> result(size);
    if (result.empty())
    {
        result.push_back(make_tuple(total, 0));
        *presult = result;
        return XAML_S_OK;
    }
    double total_star = 0;
    double total_remain = total;
    for (int32_t i = 0; i < size; i++)
    {
        xaml_ptr<xaml_object> lengths_item;
        XAML_RETURN_IF_FAILED(lengths->get_at(i, &lengths_item));
        xaml_grid_length length;
        XAML_RETURN_IF_FAILED(xaml_unbox_value(lengths_item.get(), length));
        switch (length.layout)
        {
        case xaml_grid_layout_abs:
            get<0>(result[i]) = length.value;
            total_remain -= length.value;
            break;
        case xaml_grid_layout_star:
            total_star += length.value;
            break;
        case xaml_grid_layout_auto:
        {
            double compact;
            XAML_RETURN_IF_FAILED(get_max_compact(i, children, vertical, &compact));
            get<0>(result[i]) = compact;
            total_remain -= get<0>(result[i]);
            break;
        }
        }
    }
    for (int32_t i = 0; i < size; i++)
    {
        xaml_ptr<xaml_object> lengths_item;
        XAML_RETURN_IF_FAILED(lengths->get_at(i, &lengths_item));
        xaml_grid_length length;
        XAML_RETURN_IF_FAILED(xaml_unbox_value(lengths_item.get(), length));
        if (length.layout == xaml_grid_layout_star)
        {
            get<0>(result[i]) = total_remain * length.value / total_star;
        }
        if (i)
        {
            get<1>(result[i]) = get<0>(result[i - 1]) + get<1>(result[i - 1]);
        }
    }
    *presult = result;
    return XAML_S_OK;
}

static xaml_result get_real_region(xaml_ptr<xaml_control> const& cc, xaml_rectangle& max_region) noexcept
{
    xaml_size csize;
    XAML_RETURN_IF_FAILED(cc->get_size(&csize));
    xaml_margin cmargin;
    XAML_RETURN_IF_FAILED(cc->get_margin(&cmargin));
    double cwidth = csize.width + cmargin.left + cmargin.right;
    cwidth = (min)(cwidth, max_region.width);
    xaml_halignment chalign;
    XAML_RETURN_IF_FAILED(cc->get_halignment(&chalign));
    switch (chalign)
    {
    case xaml_halignment_left:
        max_region.width = cwidth;
        break;
    case xaml_halignment_center:
        max_region.x += (max_region.width - cwidth) / 2;
        max_region.width = cwidth;
        break;
    case xaml_halignment_right:
        max_region.x += max_region.width - cwidth;
        max_region.width = cwidth;
        break;
    default:
        break;
    }
    double cheight = csize.height + cmargin.top + cmargin.bottom;
    cheight = (min)(cheight, max_region.height);
    xaml_valignment cvalign;
    XAML_RETURN_IF_FAILED(cc->get_valignment(&cvalign));
    switch (cvalign)
    {
    case xaml_valignment_top:
        max_region.height = cheight;
        break;
    case xaml_valignment_center:
        max_region.y += (max_region.height - cheight) / 2;
        max_region.height = cheight;
        break;
    case xaml_valignment_bottom:
        max_region.y += max_region.height - cheight;
        max_region.height = cheight;
        break;
    default:
        break;
    }
    return XAML_S_OK;
}

struct xaml_grid_index
{
    int32_t column, row;
    int32_t column_span, row_span;
};

unordered_map<xaml_control*, xaml_grid_index> s_grid_indecies{};

static inline double real_length_plus(double lhs, tuple<double, double> const& rhs) { return lhs + get<0>(rhs); }

xaml_result xaml_grid_impl::draw_impl(xaml_rectangle const& region, function<xaml_result(xaml_control*, xaml_rectangle const&)> const& func)
{
    XAML_RETURN_IF_FAILED(xaml_layout_base_implement::draw_impl(region, func));
    xaml_rectangle real = region - m_margin;
    vector<tuple<double, double>> columns, rows;
    XAML_RETURN_IF_FAILED(get_real_length(m_columns, m_children, real.width, false, &columns));
    XAML_RETURN_IF_FAILED(get_real_length(m_rows, m_children, real.height, true, &rows));
    XAML_FOREACH_START(c, m_children);
    {
        xaml_ptr<xaml_control> cc;
        XAML_RETURN_IF_FAILED(c->query(&cc));
        xaml_grid_index const& index = s_grid_indecies[cc.get()];
        double subx = get<1>(columns[(min)((size_t)index.column, columns.size() - 1)]) + real.x;
        double suby = get<1>(rows[(min)((size_t)index.row, rows.size() - 1)]) + real.y;
        double subw = accumulate(columns.begin() + (min)((size_t)index.column, columns.size() - 1), columns.begin() + (min)((size_t)index.column + (max<int32_t>)(index.column_span, 1), columns.size()), 0.0, real_length_plus);
        double subh = accumulate(rows.begin() + (min)((size_t)index.row, rows.size() - 1), rows.begin() + (min)((size_t)index.row + (max<int32_t>)(index.row_span, 1), rows.size()), 0.0, real_length_plus);
        xaml_rectangle subrect = { subx, suby, subw, subh };
        XAML_RETURN_IF_FAILED(get_real_region(cc, subrect));
        XAML_RETURN_IF_FAILED(cc->draw(subrect));
        if (func) func(cc.get(), subrect);
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}
