#include <xaml/ui/controls/layout_base.hpp>

using namespace std;

namespace xaml
{
    void layout_base::__draw_impl(rectangle const& region, function<void(shared_ptr<control>, rectangle const&)> func)
    {
        for (auto& c : get_children())
        {
            if (!c->get_handle()) c->__draw(rectangle{ 0, 0, 0, 0 } + c->get_margin());
            c->__size_to_fit();
        }
    }
} // namespace xaml
