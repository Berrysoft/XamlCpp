#ifndef XAML_UI_CONTROLS_SHARED_TEXT_BOX_HPP
#define XAML_UI_CONTROLS_SHARED_TEXT_BOX_HPP

#include <atomic>
#include <shared/control.hpp>
#include <xaml/ui/controls/text_box.h>

struct xaml_text_box_internal : xaml_control_internal
{
    std::atomic_bool m_text_changing{ false };

    XAML_EVENT_IMPL(text_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(text)

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

    virtual xaml_result XAML_CALL draw_text() noexcept;

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL size_to_fit() noexcept override;

    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const&, LRESULT*) noexcept override;
#elif defined(XAML_UI_COCOA)
    void on_changed() noexcept;
#elif defined(XAML_UI_GTK3)
    static void on_changed(GtkTextBuffer*, xaml_text_box_internal*) noexcept;
#elif defined(XAML_UI_QT)
    void on_text_changed_event() noexcept;
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_text_box_impl : xaml_control_implement<xaml_text_box_impl, xaml_text_box_internal, xaml_text_box>
{
    XAML_EVENT_INTERNAL_IMPL(text_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(text, xaml_string)
};

#endif // !XAML_UI_CONTROLS_SHARED_TEXT_BOX_HPP
