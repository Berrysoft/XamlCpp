#include <cmath>
#include <shared/uniform_grid.hpp>
#include <xaml/ui/controls/uniform_grid.h>

using namespace std;

xaml_result xaml_uniform_grid_internal::draw_impl(xaml_rectangle const& region, __xaml_function_wrapper_t<xaml_result(xaml_control*, xaml_rectangle const&) noexcept> func) noexcept
{
    int32_t size;
    XAML_RETURN_IF_FAILED(m_children->get_size(&size));
    if (size)
    {
        XAML_RETURN_IF_FAILED(xaml_layout_base_internal::draw_impl(region, func));
        xaml_rectangle real = region - m_margin;
        int32_t cs = m_columns;
        int32_t rs = m_rows;
        int32_t n = size;
        if (cs > 0 && rs > 0)
        {
            if (m_orientation == xaml_orientation_vertical)
            {
                rs = (n + cs - 1) / cs;
            }
            else
            {
                cs = (n + rs - 1) / rs;
            }
        }
        else
        {
            if (cs > 0)
            {
                rs = (n + cs - 1) / cs;
            }
            else if (rs > 0)
            {
                cs = (n + rs - 1) / rs;
            }
            else
            {
                if (m_orientation == xaml_orientation_vertical)
                {
                    cs = (int32_t)sqrt(n);
                    rs = (n + cs - 1) / cs;
                }
                else
                {
                    rs = (int32_t)sqrt(n);
                    cs = (n + rs - 1) / rs;
                }
            }
        }
        double w = real.width / cs;
        double h = real.height / rs;
        int32_t x = 0, y = 0;
        if (m_orientation == xaml_orientation_vertical)
        {
            XAML_FOREACH_START(xaml_control, cc, m_children);
            {
                xaml_rectangle subrect = { real.x + x * w, real.y + y * h, w, h };
                XAML_RETURN_IF_FAILED(cc->draw(subrect));
                if (func) func(cc, subrect);
                y++;
                if (y >= rs)
                {
                    y = 0;
                    x++;
                }
            }
            XAML_FOREACH_END();
        }
        else
        {
            XAML_FOREACH_START(xaml_control, cc, m_children);
            {
                xaml_rectangle subrect = { real.x + x * w, real.y + y * h, w, h };
                XAML_RETURN_IF_FAILED(cc->draw(subrect));
                if (func) func(cc, subrect);
                x++;
                if (x >= cs)
                {
                    x = 0;
                    y++;
                }
            }
            XAML_FOREACH_END();
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_uniform_grid_internal::size_to_fit() noexcept
{
    int32_t cs, rs;
    int32_t n;
    XAML_RETURN_IF_FAILED(m_children->get_size(&n));
    if (m_orientation == xaml_orientation_vertical)
    {
        cs = (int32_t)sqrt(n);
        rs = (n + cs - 1) / cs;
    }
    else
    {
        rs = (int32_t)sqrt(n);
        cs = (n + rs - 1) / rs;
    }
    double mw = 0, mh = 0;
    XAML_FOREACH_START(xaml_control, cc, m_children);
    {
        XAML_RETURN_IF_FAILED(cc->size_to_fit());
        xaml_size csize;
        XAML_RETURN_IF_FAILED(cc->get_size(&csize));
        xaml_margin cmargin;
        XAML_RETURN_IF_FAILED(cc->get_margin(&cmargin));
        csize.width += cmargin.left + cmargin.right;
        csize.height += cmargin.top + cmargin.bottom;
        if (csize.width > mw) mw = csize.width;
        if (csize.height > mh) mh = csize.height;
    }
    XAML_FOREACH_END();
    return set_size_noevent({ mw * cs, mh * rs });
}

xaml_result XAML_CALL xaml_uniform_grid_new(xaml_uniform_grid** ptr) noexcept
{
    return xaml_object_init<xaml_uniform_grid_impl>(ptr);
}

xaml_result XAML_CALL xaml_uniform_grid_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_uniform_grid;
    XAML_RETURN_IF_FAILED(xaml_layout_base_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_uniform_grid_new);
    XAML_TYPE_INFO_ADD_PROP(columns, int32_t);
    XAML_TYPE_INFO_ADD_PROP(rows, int32_t);
    XAML_TYPE_INFO_ADD_PROP(orientation, xaml_orientation);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_uniform_grid_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_uniform_grid, "xaml/ui/controls/uniform_grid.h");
    XAML_RETURN_IF_FAILED(xaml_uniform_grid_members(__info));
    return ctx->add_type(__info);
}
