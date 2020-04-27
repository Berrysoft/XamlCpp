#ifndef XAML_UI_CONTROLS_SHARED_LABEL_HPP
#define XAML_UI_CONTROLS_SHARED_LABEL_HPP

#include <shared/control.hpp>
#include <xaml/ui/controls/label.h>

struct xaml_label_impl : xaml_control_implement<xaml_label_impl, xaml_label>
{
    XAML_EVENT_IMPL(text_changed)
    XAML_PROP_PTR_EVENT_IMPL(text, xaml_string)

    XAML_PROP_IMPL(text_halignment, xaml_halignment, xaml_halignment*, xaml_halignment)

    virtual xaml_result XAML_CALL draw_text() noexcept;
    virtual xaml_result XAML_CALL draw_alignment() noexcept;

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;
    xaml_result XAML_CALL size_to_fit() noexcept override;

    xaml_result XAML_CALL init() noexcept override;
};

#endif // !XAML_UI_CONTROLS_SHARED_LABEL_HPP
