#include <xaml/ui/control.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_drawing.hpp>

namespace xaml
{
    void control::__set_rect(rectangle const& region)
    {
        rectangle real = region - get_margin();
        NSView* view = get_handle()->handle;
        NSView* parent_view = get_parent().lock()->get_handle()->handle;
        NSRect frame = view.frame;
        NSRect parent_frame = parent_view.frame;
        frame.origin = { real.x, parent_frame.size.height - real.height - real.y };
        frame.size = { real.width, real.height };
        view.frame = frame;
        __set_size_noevent({ real.width, real.height });
    }

    void control::__size_to_fit()
    {
        NSView* view = get_handle()->handle;
        if ([view isKindOfClass:[NSControl class]])
        {
            NSControl* control = (NSControl*)view;
            [control sizeToFit];
            NSRect frame = view.frame;
            __set_size_noevent(from_native(frame.size));
        }
    }

    void control::draw_size()
    {
        NSView* view = get_handle()->handle;
        NSRect frame = view.frame;
        frame.size = to_native<NSSize>(get_size());
        view.frame = frame;
    }

    void control::draw_visible()
    {
        NSView* view = get_handle()->handle;
        view.hidden = !m_is_visible;
	}
}
