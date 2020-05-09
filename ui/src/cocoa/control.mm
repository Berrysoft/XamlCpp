#include <shared/control.hpp>
#include <xaml/ui/control.h>
#include <xaml/ui/native_drawing.hpp>

using namespace std;

xaml_result xaml_control_internal::set_rect(xaml_rectangle const& region) noexcept
{
    xaml_rectangle real = region - m_margin;
    NSView* view = m_handle;
    if (m_parent)
    {
        xaml_ptr<xaml_cocoa_control> native_parent;
        if (XAML_SUCCEEDED(m_parent->query(&native_parent)))
        {
            NSView* parent_view;
            XAML_RETURN_IF_FAILED(native_parent->get_handle(&parent_view));
            NSRect frame = view.frame;
            NSRect const& parent_frame = parent_view.frame;
            frame.origin = { real.x, parent_frame.size.height - real.height - real.y };
            frame.size = { real.width, real.height };
            view.frame = frame;
            XAML_RETURN_IF_FAILED(set_size_noevent({ real.width, real.height }));
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_control_internal::size_to_fit() noexcept
{
    NSView* view = m_handle;
    if ([view isKindOfClass:[NSControl class]])
    {
        NSControl* control = (NSControl*)view;
        [control sizeToFit];
        NSRect const& frame = view.frame;
        XAML_RETURN_IF_FAILED(set_size_noevent(xaml_from_native(frame.size)));
    }
    return XAML_S_OK;
}

xaml_result xaml_control_internal::draw_size() noexcept
{
    NSView* view = m_handle;
    NSRect frame = view.frame;
    frame.size = xaml_to_native<NSSize>(m_size);
    view.frame = frame;
    return XAML_S_OK;
}

xaml_result xaml_control_internal::draw_visible() noexcept
{
    NSView* view = m_handle;
    view.hidden = !m_is_visible;
    return XAML_S_OK;
}
