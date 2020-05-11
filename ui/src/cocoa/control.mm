#include <shared/control.hpp>
#include <xaml/ui/control.h>
#include <xaml/ui/native_drawing.hpp>

using namespace std;

xaml_result xaml_control_internal::set_rect(xaml_rectangle const& region) noexcept
{
    xaml_rectangle real = region - m_margin;
    if (m_parent)
    {
        xaml_ptr<xaml_cocoa_control> native_parent;
        if (XAML_SUCCEEDED(m_parent->query(&native_parent)))
        {
            NSView* parent_view;
            XAML_RETURN_IF_FAILED(native_parent->get_handle(&parent_view));
            NSRect frame = m_handle.frame;
            NSRect const& parent_frame = parent_view.frame;
            frame.origin = { real.x, parent_frame.size.height - real.height - real.y };
            frame.size = { real.width, real.height };
            m_handle.frame = frame;
            XAML_RETURN_IF_FAILED(set_size_noevent({ real.width, real.height }));
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_control_internal::size_to_fit() noexcept
{
    if ([m_handle isKindOfClass:[NSControl class]])
    {
        NSControl* control = (NSControl*)m_handle;
        [control sizeToFit];
        NSRect const& frame = m_handle.frame;
        XAML_RETURN_IF_FAILED(set_size_noevent(xaml_from_native(frame.size)));
    }
    return XAML_S_OK;
}

xaml_result xaml_control_internal::draw_size() noexcept
{
    NSRect frame = m_handle.frame;
    frame.size = xaml_to_native<NSSize>(m_size);
    m_handle.frame = frame;
    return XAML_S_OK;
}

xaml_result xaml_control_internal::draw_visible() noexcept
{
    m_handle.hidden = !m_is_visible;
    return XAML_S_OK;
}
