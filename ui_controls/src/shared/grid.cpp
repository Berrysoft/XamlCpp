#include <algorithm>
#include <numeric>
#include <shared/grid.hpp>
#include <xaml/ui/controls/grid.h>

using namespace std;

xaml_result xaml_grid_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_layout_base_internal::init());
    XAML_RETURN_IF_FAILED(xaml_vector_new(&m_columns));
    XAML_RETURN_IF_FAILED(xaml_vector_new(&m_rows));
    return XAML_S_OK;
}

static xaml_result get_max_compact(int32_t index, xaml_ptr<xaml_vector<xaml_control>> const& children, bool vertical, double* presult) noexcept
{
    double result = 0;
    XAML_FOREACH_START(xaml_control, cc, children);
    {
        int32_t crow, ccol, crows, ccols;
        XAML_RETURN_IF_FAILED(xaml_grid_get_column(cc, &ccol));
        XAML_RETURN_IF_FAILED(xaml_grid_get_row(cc, &crow));
        XAML_RETURN_IF_FAILED(xaml_grid_get_column_span(cc, &ccols));
        XAML_RETURN_IF_FAILED(xaml_grid_get_row_span(cc, &crows));
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

static xaml_result get_real_length(xaml_ptr<xaml_vector<xaml_grid_length>> const& lengths, xaml_ptr<xaml_vector<xaml_control>> const& children, double total, bool vertical, vector<tuple<double, double>>* presult) noexcept
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
        xaml_grid_length length;
        XAML_RETURN_IF_FAILED(lengths->get_at(i, &length));
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
        xaml_grid_length length;
        XAML_RETURN_IF_FAILED(lengths->get_at(i, &length));
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

xaml_result xaml_grid_internal::draw_impl(xaml_rectangle const& region, __xaml_function_wrapper_t<xaml_result(xaml_control*, xaml_rectangle const&) noexcept> func) noexcept
{
    XAML_RETURN_IF_FAILED(xaml_layout_base_internal::draw_impl(region, func));
    xaml_rectangle real = region - m_margin;
    vector<tuple<double, double>> columns, rows;
    XAML_RETURN_IF_FAILED(get_real_length(m_columns, m_children, real.width, false, &columns));
    XAML_RETURN_IF_FAILED(get_real_length(m_rows, m_children, real.height, true, &rows));
    XAML_FOREACH_START(xaml_control, cc, m_children);
    {
        xaml_grid_index const& index = s_grid_indecies[cc];
        double subx = get<1>(columns[(min)((size_t)index.column, columns.size() - 1)]) + real.x;
        double suby = get<1>(rows[(min)((size_t)index.row, rows.size() - 1)]) + real.y;
        double subw = accumulate(columns.begin() + (min)((size_t)index.column, columns.size() - 1), columns.begin() + (min)((size_t)index.column + (max<int32_t>)(index.column_span, 1), columns.size()), 0.0, real_length_plus);
        double subh = accumulate(rows.begin() + (min)((size_t)index.row, rows.size() - 1), rows.begin() + (min)((size_t)index.row + (max<int32_t>)(index.row_span, 1), rows.size()), 0.0, real_length_plus);
        xaml_rectangle subrect = { subx, suby, subw, subh };
        XAML_RETURN_IF_FAILED(get_real_region(cc, subrect));
        XAML_RETURN_IF_FAILED(cc->draw(subrect));
        if (func) func(cc, subrect);
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}

template <>
struct __xaml_converter<xaml_ptr<xaml_vector<xaml_grid_length>>, void>
{
    xaml_result operator()(xaml_ptr<xaml_object> const& obj, xaml_vector<xaml_grid_length>** ptr) const noexcept
    {
        if (auto vec = obj.query<xaml_vector<xaml_grid_length>>())
        {
            return vec->query(ptr);
        }
        else if (auto s = obj.query<xaml_string>())
        {
            std::string_view str;
            XAML_RETURN_IF_FAILED(to_string_view(s, &str));
            constexpr char __delimeter[] = " ,\t\r\n";
            constexpr char __auto[] = "auto";
            xaml_ptr<xaml_vector<xaml_grid_length>> result;
            XAML_RETURN_IF_FAILED(xaml_vector_new(&result));
            std::size_t offset = 0;
            do
            {
                std::size_t index = str.find_first_of(__delimeter, offset);
                std::string_view lenstr = str.substr(offset, index - offset);
                xaml_grid_length len;
                if (lenstr == __auto)
                {
                    len = { 0, xaml_grid_layout_auto };
                }
                else if (lenstr.back() == '*')
                {
                    double rate = __stof<double>(lenstr.substr(0, lenstr.length() - 1));
                    len = { rate, xaml_grid_layout_star };
                }
                else
                {
                    double value = __stof<double>(lenstr);
                    len = { value, xaml_grid_layout_abs };
                }
                XAML_RETURN_IF_FAILED(result->append(len));
                offset = str.find_first_not_of(__delimeter, index);
            } while (offset != std::string_view::npos);
            return result->query(ptr);
        }
        else
        {
            return XAML_E_INVALIDARG;
        }
    }
};

xaml_result XAML_CALL xaml_grid_new(xaml_grid** ptr) noexcept
{
    return xaml_object_init<xaml_grid_impl>(ptr);
}

xaml_result XAML_CALL xaml_grid_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_grid;
    XAML_RETURN_IF_FAILED(xaml_layout_base_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_grid_new);
    XAML_TYPE_INFO_ADD_PROP(columns, xaml_vector<xaml_grid_length>);
    XAML_TYPE_INFO_ADD_PROP(rows, xaml_vector<xaml_grid_length>);
    XAML_TYPE_INFO_ADD_CPROP(column, xaml_grid_length);
    XAML_TYPE_INFO_ADD_CPROP(row, xaml_grid_length);
    XAML_TYPE_INFO_ADD_APROP(xaml_grid, column, int32_t);
    XAML_TYPE_INFO_ADD_APROP(xaml_grid, row, int32_t);
    XAML_TYPE_INFO_ADD_APROP(xaml_grid, column_span, int32_t);
    XAML_TYPE_INFO_ADD_APROP(xaml_grid, row_span, int32_t);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_grid_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_grid, "xaml/ui/controls/grid.h");
    XAML_RETURN_IF_FAILED(xaml_grid_members(__info));
    return ctx->add_type(__info);
}

xaml_result XAML_CALL xaml_grid_get_column(xaml_control* c, XAML_STD int32_t* presult) noexcept
{
    *presult = s_grid_indecies[c].column;
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_grid_set_column(xaml_control* c, XAML_STD int32_t value) noexcept
{
    s_grid_indecies[c].column = value;
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_grid_get_row(xaml_control* c, XAML_STD int32_t* presult) noexcept
{
    *presult = s_grid_indecies[c].row;
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_grid_set_row(xaml_control* c, XAML_STD int32_t value) noexcept
{
    s_grid_indecies[c].row = value;
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_grid_get_column_span(xaml_control* c, XAML_STD int32_t* presult) noexcept
{
    *presult = s_grid_indecies[c].column_span;
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_grid_set_column_span(xaml_control* c, XAML_STD int32_t value) noexcept
{
    s_grid_indecies[c].column_span = value;
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_grid_get_row_span(xaml_control* c, XAML_STD int32_t* presult) noexcept
{
    *presult = s_grid_indecies[c].row_span;
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_grid_set_row_span(xaml_control* c, XAML_STD int32_t value) noexcept
{
    s_grid_indecies[c].row_span = value;
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_grid_layout_register(xaml_meta_context* ctx) noexcept
{
    XAML_ENUM_INFO_MAP_NEW();
    XAML_ENUM_INFO_ADD2(xaml_grid_layout, abs);
    XAML_ENUM_INFO_ADD2(xaml_grid_layout, star);
    XAML_ENUM_INFO_ADD2(xaml_grid_layout, auto);
    XAML_ENUM_INFO_NEW(xaml_grid_layout, "xaml/ui/controls/grid.h");
    return ctx->add_type(__info);
}
