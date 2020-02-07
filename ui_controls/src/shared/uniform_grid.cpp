#include <cmath>
#include <xaml/ui/controls/uniform_grid.hpp>

using namespace std;

namespace xaml
{
    uniform_grid::uniform_grid() : layout_base() {}

    uniform_grid::~uniform_grid() {}

    void uniform_grid::__draw_impl(rectangle const& region, function<void(shared_ptr<control>, rectangle const&)> func)
    {
        if (!m_children.empty())
        {
            layout_base::__draw_impl(region, func);
            rectangle real = region - get_margin();
            size_t cs = get_columns();
            size_t rs = get_rows();
            size_t n = m_children.size();
            if (cs > 0 && rs > 0)
            {
                if (get_orientation() == orientation::vertical)
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
                    if (get_orientation() == orientation::vertical)
                    {
                        cs = (size_t)sqrt(n);
                        rs = (n + cs - 1) / cs;
                    }
                    else
                    {
                        rs = (size_t)sqrt(n);
                        cs = (n + rs - 1) / rs;
                    }
                }
            }
            double w = real.width / cs;
            double h = real.height / rs;
            size_t x = 0, y = 0;
            if (get_orientation() == orientation::vertical)
            {
                for (auto& c : m_children)
                {
                    rectangle subrect = { real.x + x * w, real.y + y * h, w, h };
                    c->__draw(subrect);
                    if (func) func(c, subrect);
                    y++;
                    if (y >= rs)
                    {
                        y = 0;
                        x++;
                    }
                }
            }
            else
            {
                for (auto& c : m_children)
                {
                    rectangle subrect = { real.x + x * w, real.y + y * h, w, h };
                    c->__draw(subrect);
                    if (func) func(c, subrect);
                    x++;
                    if (x >= cs)
                    {
                        x = 0;
                        y++;
                    }
                }
            }
        }
    }
} // namespace xaml
