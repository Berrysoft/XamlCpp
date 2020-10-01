#ifndef XAML_UI_CONTROLS_SHARED_CHECK_BOX_HPP
#define XAML_UI_CONTROLS_SHARED_CHECK_BOX_HPP

#include <shared/button.hpp>
#include <xaml/ui/controls/check_box.h>

struct xaml_check_box_internal : xaml_button_internal
{
    XAML_EVENT_IMPL(is_checked_changed, xaml_object, bool)
    XAML_PROP_EVENT_IMPL(is_checked, bool, bool*, bool)

    virtual xaml_result XAML_CALL draw_checked() noexcept;

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const&, LRESULT*) noexcept override;
    xaml_result XAML_CALL size_to_fit() noexcept override;
#elif defined(XAML_UI_COCOA)
    xaml_result XAML_CALL on_state_changed() noexcept;
#elif defined(XAML_UI_GTK3)
    static void on_toggled(GtkWidget*, xaml_check_box_internal*) noexcept;
#elif defined(XAML_UI_QT5)
    void on_toggled(bool) noexcept;
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_check_box_impl : xaml_button_implement<xaml_check_box_impl, xaml_check_box_internal, xaml_check_box>
{
    XAML_EVENT_INTERNAL_IMPL(is_checked_changed, xaml_object, bool)
    XAML_PROP_INTERNAL_IMPL(is_checked, bool*, bool)
};

#endif // !XAML_UI_CONTROLS_SHARED_CHECK_BOX_HPP
