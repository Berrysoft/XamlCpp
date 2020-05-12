#ifndef XAML_UI_CONTROLS_SHARED_BUTTON_HPP
#define XAML_UI_CONTROLS_SHARED_BUTTON_HPP

#include <shared/control.hpp>
#include <xaml/ui/controls/button.h>

struct xaml_button_internal : xaml_control_internal
{
    XAML_EVENT_IMPL(text_changed)
    XAML_PROP_STRING_EVENT_IMPL(text)

    XAML_EVENT_IMPL(is_default_changed)
    XAML_PROP_EVENT_IMPL(is_default, bool, bool*, bool)

    XAML_EVENT_IMPL(click)

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

    virtual xaml_result XAML_CALL draw_text() noexcept;
    virtual xaml_result XAML_CALL draw_default() noexcept;

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const& msg, LRESULT*) noexcept override;

    xaml_result XAML_CALL size_to_fit() noexcept override;
#elif defined(XAML_UI_COCOA)
    xaml_result XAML_CALL draw_size() noexcept override;

    void on_action() noexcept;
#elif defined(XAML_UI_GTK3)
    static void on_clicked(GtkWidget*, xaml_button_internal*) noexcept;
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL init() noexcept override;
};

template <typename T, typename Internal, typename... Base>
struct xaml_button_implement : xaml_control_implement<T, Internal, Base..., xaml_button>
{
    XAML_EVENT_INTERNAL_IMPL(text_changed)
    XAML_PROP_PTR_INTERNAL_IMPL(text, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(is_default_changed)
    XAML_PROP_INTERNAL_IMPL(is_default, bool*, bool)

    XAML_EVENT_INTERNAL_IMPL(click)
};

struct xaml_button_impl : xaml_button_implement<xaml_button_impl, xaml_button_internal>
{
};

#endif // !XAML_UI_CONTROLS_SHARED_BUTTON_HPP
