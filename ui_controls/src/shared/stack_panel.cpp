#include <algorithm>
#include <numeric>
#include <xaml/ui/controls/stack_panel.hpp>

using namespace std;

namespace xaml
{
    stack_panel::stack_panel() : layout_base() {}

    stack_panel::~stack_panel() {}

    void stack_panel::__draw_impl(rectangle const& region, function<void(shared_ptr<control>, rectangle const&)> func)
    {
        for (auto& c : m_children)
        {
            if (!c->get_handle()) c->__draw(rectangle{ 0, 0, 0, 0 } + c->get_margin());
            c->__size_to_fit();
        }
        rectangle real = region - get_margin();
        if (get_orientation() == orientation::vertical)
        {
            double suby = real.y;
            for (auto& c : m_children)
            {
                margin cm = c->get_margin();
                double subh = c->get_height() + cm.top + cm.bottom;
                rectangle subrect = { real.x, suby, real.width, subh };
                c->__draw(subrect);
                if (func) func(c, subrect);
                suby += subh;
            }
        }
        else
        {
            double subx = real.x;
            for (auto& c : m_children)
            {
                margin cm = c->get_margin();
                double subw = c->get_width() + cm.left + cm.right;
                rectangle subrect = { subx, real.y, subw, real.height };
                c->__draw(subrect);
                if (func) func(c, subrect);
                subx += subw;
            }
        }
    }

    void stack_panel::__size_to_fit()
    {
        if (get_orientation() == orientation::vertical)
        {
            __set_size_noevent({ get_width(), accumulate(m_children.begin(), m_children.end(), 0.0, [](double lhs, shared_ptr<control> const& rhs) { return lhs + rhs->get_height(); }) });
        }
        else
        {
            __set_size_noevent({ accumulate(m_children.begin(), m_children.end(), 0.0, [](double lhs, shared_ptr<control> const& rhs) { return lhs + rhs->get_width(); }), get_height() });
        }
    }
} // namespace xaml
