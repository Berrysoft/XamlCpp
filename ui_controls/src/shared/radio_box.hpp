#ifndef XAML_UI_CONTROLS_SHARED_RADIO_BOX_HPP
#define XAML_UI_CONTROLS_SHARED_RADIO_BOX_HPP

#include <shared/button.hpp>
#include <xaml/ui/controls/radio_box.h>

struct xaml_radio_box_impl : xaml_button_implement<xaml_radio_box_impl, xaml_radio_box>
{
    XAML_EVENT_IMPL(is_checked_changed)
    XAML_PROP_EVENT_IMPL(is_checked, bool, bool*, bool)

    XAML_PROP_PTR_IMPL(group, xaml_string)

    virtual xaml_result draw_checked() noexcept;
    virtual xaml_result draw_group() noexcept;

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const&, LRESULT*) noexcept override;
    xaml_result XAML_CALL size_to_fit() noexcept override;
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL init() noexcept override;
};

#endif // !XAML_UI_CONTROLS_SHARED_RADIO_BOX_HPP
