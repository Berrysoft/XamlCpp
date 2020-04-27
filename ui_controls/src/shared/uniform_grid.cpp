#include <cmath>
#include <shared/uniform_grid.hpp>
#include <xaml/ui/controls/uniform_grid.h>

using namespace std;

xaml_result xaml_uniform_grid_impl::draw_impl(xaml_rectangle const& region, function<xaml_result(xaml_control*, xaml_rectangle const&)> const& func) noexcept
{
    int32_t size;
    XAML_RETURN_IF_FAILED(m_children->get_size(&size));
    if (size)
    {
        XAML_RETURN_IF_FAILED(xaml_layout_base_implement::draw_impl(region, func));
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
            XAML_FOREACH_START(c, m_children);
            {
                xaml_ptr<xaml_control> cc;
                XAML_RETURN_IF_FAILED(c->query(&cc));
                xaml_rectangle subrect = { real.x + x * w, real.y + y * h, w, h };
                XAML_RETURN_IF_FAILED(cc->draw(subrect));
                if (func) func(cc.get(), subrect);
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
            XAML_FOREACH_START(c, m_children);
            {
                xaml_ptr<xaml_control> cc;
                XAML_RETURN_IF_FAILED(c->query(&cc));
                xaml_rectangle subrect = { real.x + x * w, real.y + y * h, w, h };
                XAML_RETURN_IF_FAILED(cc->draw(subrect));
                if (func) func(cc.get(), subrect);
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

xaml_result xaml_uniform_grid_impl::size_to_fit() noexcept
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
    XAML_FOREACH_START(c, m_children);
    {
        xaml_ptr<xaml_control> cc;
        XAML_RETURN_IF_FAILED(c->query(&cc));
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
