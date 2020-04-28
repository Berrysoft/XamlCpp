#ifndef XAML_UI_CONTROLS_SHARED_ENTRY_HPP
#define XAML_UI_CONTROLS_SHARED_ENTRY_HPP

#include <shared/control.hpp>
#include <xaml/ui/controls/entry.h>

struct xaml_entry_impl : xaml_control_implement<xaml_entry_impl, xaml_entry>
{
    XAML_EVENT_IMPL(text_changed)
    XAML_PROP_PTR_EVENT_IMPL(text, xaml_string)

    XAML_PROP_IMPL(text_halignment, xaml_halignment, xaml_halignment*, xaml_halignment)

    virtual xaml_result XAML_CALL draw_text() noexcept;
    virtual xaml_result XAML_CALL draw_alignment() noexcept;

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;
    xaml_result XAML_CALL size_to_fit() noexcept override;

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const&, LRESULT*) noexcept override;
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL init() noexcept override;
};

#endif // !XAML_UI_CONTROLS_SHARED_ENTRY_HPP
