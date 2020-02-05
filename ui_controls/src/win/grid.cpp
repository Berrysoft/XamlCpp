#include <xaml/ui/controls/grid.hpp>

using namespace std;

namespace xaml
{
    optional<LRESULT> grid::__wnd_proc(window_message const& msg)
    {
        optional<LRESULT> result = nullopt;
        for (auto c : m_children)
        {
            auto r = c->__wnd_proc(msg);
            if (r) result = r;
        }
        return result;
    }

    void grid::__draw(rectangle const& region)
    {
        bool new_draw = !get_handle();
        set_handle(get_parent()->get_handle());
        SendMessage(get_handle(), WM_SETREDRAW, FALSE, 0);
        draw_impl(new_draw, region);
        SendMessage(get_handle(), WM_SETREDRAW, TRUE, 0);
    }
} // namespace xaml
