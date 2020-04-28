#ifndef XAML_UI_CONTROLS_SHARED_PROGRESS_HPP
#define XAML_UI_CONTROLS_SHARED_PROGRESS_HPP

#include <shared/control.hpp>
#include <xaml/ui/controls/progress.h>

struct xaml_progress_impl : xaml_control_implement<xaml_progress_impl, xaml_progress>
{
    XAML_EVENT_IMPL(value_changed)
    XAML_PROP_EVENT_IMPL(value, std::int32_t, std::int32_t*, std::int32_t)

    XAML_PROP_IMPL(minimum, std::int32_t, std::int32_t*, std::int32_t)
    XAML_PROP_IMPL(maximum, std::int32_t, std::int32_t*, std::int32_t)

    XAML_EVENT_IMPL(is_indeterminate_changed)
    XAML_PROP_EVENT_IMPL(is_indeterminate, bool, bool*, bool)

    virtual xaml_result XAML_CALL draw_progress() noexcept;
    virtual xaml_result XAML_CALL draw_indeterminate() noexcept;

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL size_to_fit() noexcept override;
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL init() noexcept override;
};

#endif // !XAML_UI_CONTROLS_SHARED_PROGRESS_HPP
