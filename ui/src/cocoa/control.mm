#include <xaml/ui/control.hpp>

namespace xaml
{
    control::~control()
    {
    }

    void control::__set_rect(rectangle const& real)
    {
        NSView* view = get_handle();
        NSView* parent_view = get_parent().lock()->get_handle();
        NSRect frame = view.frame;
        NSRect parent_frame = parent_view.frame;
        frame.origin = { real.x, parent_frame.size.height - real.height - real.y };
        frame.size = { real.width, real.height };
        view.frame = frame;
        __set_size_noevent({ real.width, real.height });
    }
}
