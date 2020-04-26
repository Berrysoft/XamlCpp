#include <algorithm>
#include <numeric>
#include <shared/stack_panel.hpp>
#include <xaml/ui/controls/stack_panel.h>

using namespace std;

xaml_result xaml_stack_panel_impl::draw_impl(xaml_rectangle const& region, function<xaml_result(xaml_control*, xaml_rectangle const&)> func) noexcept
{
    XAML_RETURN_IF_FAILED(xaml_layout_base_implement::draw_impl(region, func));
    xaml_rectangle real = region - m_margin;
    if (m_orientation == xaml_orientation_vertical)
    {
        double suby = real.y;
        for (auto c : m_children)
        {
            xaml_ptr<xaml_control> cc;
            XAML_RETURN_IF_FAILED(c->query(&cc));
            xaml_margin cm;
            XAML_RETURN_IF_FAILED(cc->get_margin(&cm));
            xaml_size cs;
            XAML_RETURN_IF_FAILED(cc->get_size(&cs));
            double subh = cs.height + cm.top + cm.bottom;
            xaml_rectangle subrect = { real.x, suby, real.width, subh };
            XAML_RETURN_IF_FAILED(cc->draw(subrect));
            if (func) XAML_RETURN_IF_FAILED(func(cc.get(), subrect));
            suby += subh;
        }
    }
    else
    {
        double subx = real.x;
        for (auto c : m_children)
        {
            xaml_ptr<xaml_control> cc;
            XAML_RETURN_IF_FAILED(c->query(&cc));
            xaml_margin cm;
            XAML_RETURN_IF_FAILED(cc->get_margin(&cm));
            xaml_size cs;
            XAML_RETURN_IF_FAILED(cc->get_size(&cs));
            double subw = cs.width + cm.left + cm.right;
            xaml_rectangle subrect = { subx, real.y, subw, real.height };
            XAML_RETURN_IF_FAILED(cc->draw(subrect));
            if (func) XAML_RETURN_IF_FAILED(func(cc.get(), subrect));
            subx += subw;
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_stack_panel_impl::size_to_fit() noexcept
{
    if (m_orientation == xaml_orientation_vertical)
    {
        double height = 0;
        for (auto c : m_children)
        {
            xaml_ptr<xaml_control> cc;
            XAML_RETURN_IF_FAILED(c->query(&cc));
            xaml_margin cm;
            XAML_RETURN_IF_FAILED(cc->get_margin(&cm));
            xaml_size cs;
            XAML_RETURN_IF_FAILED(cc->get_size(&cs));
            height += cs.height + cm.top + cm.bottom;
        }
        return set_size_noevent({ m_size.width, height });
    }
    else
    {
        double width = 0;
        for (auto c : m_children)
        {
            xaml_ptr<xaml_control> cc;
            XAML_RETURN_IF_FAILED(c->query(&cc));
            xaml_margin cm;
            XAML_RETURN_IF_FAILED(cc->get_margin(&cm));
            xaml_size cs;
            XAML_RETURN_IF_FAILED(cc->get_size(&cs));
            width += cs.width + cm.left + cm.right;
        }
        return set_size_noevent({ width, m_size.height });
    }
}
