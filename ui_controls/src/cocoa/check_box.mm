#include <shared/check_box.hpp>
#include <xaml/ui/controls/check_box.h>

using namespace std;

xaml_result xaml_check_box_internal::draw(xaml_rectangle const& region) noexcept
{
    bool new_draw = !m_handle;
    XAML_RETURN_IF_FAILED(xaml_button_internal::draw(region));
    if (new_draw)
    {
        NSMatrix* matrix = (NSMatrix*)m_handle;
        NSButtonCell* button = (NSButtonCell*)[matrix.cells objectAtIndex:0];
        [button setButtonType:NSButtonTypeSwitch];
        XAML_RETURN_IF_FAILED(draw_checked());
    }
    return XAML_S_OK;
}

xaml_result xaml_check_box_internal::draw_checked() noexcept
{
    NSMatrix* matrix = (NSMatrix*)m_handle;
    NSButtonCell* button = (NSButtonCell*)[matrix.cells objectAtIndex:0];
    button.state = m_is_checked ? NSControlStateValueOn : NSControlStateValueOff;
    return XAML_S_OK;
}

xaml_result xaml_check_box_internal::on_state_changed() noexcept
{
    NSMatrix* matrix = (NSMatrix*)m_handle;
    NSButtonCell* button = (NSButtonCell*)[matrix.cells objectAtIndex:0];
    return set_is_checked(button.state == NSControlStateValueOn);
}
