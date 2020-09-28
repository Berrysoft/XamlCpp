#ifndef XAML_UI_CONTROLS_SHARED_ENTRY_HPP
#define XAML_UI_CONTROLS_SHARED_ENTRY_HPP

#include <atomic>
#include <shared/control.hpp>
#include <xaml/ui/controls/entry.h>

struct xaml_entry_internal : xaml_control_internal
{
    std::atomic_bool m_text_changing{ false };

    XAML_EVENT_IMPL(text_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(text)

    XAML_PROP_IMPL(text_halignment, xaml_halignment, xaml_halignment*, xaml_halignment)

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

    virtual xaml_result XAML_CALL draw_text() noexcept;
    virtual xaml_result XAML_CALL draw_alignment() noexcept;

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL size_to_fit() noexcept override;

    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const&, LRESULT*) noexcept override;
#elif defined(XAML_UI_COCOA)
    void on_changed() noexcept;
#elif defined(XAML_UI_GTK3)
    xaml_result XAML_CALL size_to_fit() noexcept override;

    static void on_changed(GtkWidget*, xaml_entry_internal*) noexcept;
#elif defined(XAML_UI_QT5)
    void on_text_changed_event(QString const&) noexcept;
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL init() noexcept override;
};

template <typename T, typename Internal, typename Base>
struct xaml_entry_implement : xaml_control_implement<T, Internal, Base>
{
    XAML_EVENT_INTERNAL_IMPL(text_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(text, xaml_string)

    XAML_PROP_INTERNAL_IMPL(text_halignment, xaml_halignment*, xaml_halignment)
};

struct xaml_entry_impl : xaml_entry_implement<xaml_entry_impl, xaml_entry_internal, xaml_entry>
{
};

#endif // !XAML_UI_CONTROLS_SHARED_ENTRY_HPP
