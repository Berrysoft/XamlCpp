#ifndef XAML_UI_CONTROLS_SHARED_STACK_PANEL_HPP
#define XAML_UI_CONTROLS_SHARED_STACK_PANEL_HPP

#include <shared/layout_base.hpp>
#include <xaml/ui/controls/stack_panel.h>

struct xaml_stack_panel_impl : xaml_layout_base_implement<xaml_stack_panel_impl, xaml_stack_panel>
{
    XAML_PROP_IMPL(orientation, xaml_orientation, xaml_orientation*, xaml_orientation)

    xaml_result XAML_CALL draw_impl(xaml_rectangle const&, std::function<xaml_result(xaml_control*, xaml_rectangle const&)> const&) noexcept override;
    xaml_result XAML_CALL size_to_fit() noexcept override;
};

#endif // !XAML_UI_CONTROLS_SHARED_STACK_PANEL_HPP
