#ifndef XAML_UI_CONTROLS_SHARED_CHECK_BOX_HPP
#define XAML_UI_CONTROLS_SHARED_CHECK_BOX_HPP

#include <shared/button.hpp>
#include <xaml/ui/controls/check_box.h>

struct xaml_check_box_impl : xaml_button_implement<xaml_check_box_impl, xaml_check_box>
{
    XAML_EVENT_IMPL(is_checked_changed)
    XAML_PROP_EVENT_IMPL(is_checked, bool, bool*, bool)

    virtual xaml_result draw_checked() noexcept;

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

#ifdef XAML_UI_WINDOWS
    xaml_result wnd_proc(xaml_win32_window_message const&, LRESULT*) noexcept override;
    xaml_result XAML_CALL size_to_fit() noexcept override;
#endif // XAML_UI_WINDOWS

    xaml_result init() noexcept override;
};

#endif // !XAML_UI_CONTROLS_SHARED_CHECK_BOX_HPP