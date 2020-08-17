#include <shared/control.hpp>
#include <xaml/ui/control.h>
#include <xaml/ui/drawing_conv.hpp>

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
            if (![parent_view.subviews containsObject:m_handle])
            {
                [parent_view addSubview:m_handle];
            }
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

void xaml_control_internal::on_mouse_down_event(xaml_mouse_button button) noexcept
{
    XAML_ASSERT_SUCCEEDED(on_mouse_down(m_outer_this, button));
}

void xaml_control_internal::on_mouse_up_event(xaml_mouse_button button) noexcept
{
    XAML_ASSERT_SUCCEEDED(on_mouse_up(m_outer_this, button));
}

void xaml_control_internal::on_mouse_moved_event(xaml_point const& p) noexcept
{
    xaml_point realp = { p.x, m_size.height - p.y };
    XAML_ASSERT_SUCCEEDED(on_mouse_move(m_outer_this, realp));
}
