#include <xaml/ui/control.hpp>

namespace xaml
{
    control::~control()
    {
    }

    void control::__set_rect(rectangle const& real)
    {
        NSView* view = get_parent()->get_handle();
        NSRect parent_frame = [view frame];
        NSRect frame = get_handle().frame;
        frame.origin = { real.x, parent_frame.size.height - real.height - real.y };
        frame.size = { real.width, real.height };
        get_handle().frame = frame;
        __set_size_noevent({ real.width, real.height });
	}
}
