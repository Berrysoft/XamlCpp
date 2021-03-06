#ifndef XAML_UI_CONTROLS_SHARED_STACK_PANEL_HPP
#define XAML_UI_CONTROLS_SHARED_STACK_PANEL_HPP

#include <shared/layout_base.hpp>
#include <xaml/ui/controls/stack_panel.h>

struct xaml_stack_panel_internal : xaml_layout_base_internal
{
    XAML_PROP_IMPL(orientation, xaml_orientation, xaml_orientation*, xaml_orientation)

    xaml_result XAML_CALL draw_impl(xaml_rectangle const&, __xaml_function_view_wrapper_t<xaml_result(xaml_control*, xaml_rectangle const&) noexcept>) noexcept override;
    xaml_result XAML_CALL size_to_fit() noexcept override;
};

struct xaml_stack_panel_impl : xaml_layout_base_implement<xaml_stack_panel_impl, xaml_stack_panel_internal, xaml_stack_panel>
{
    XAML_PROP_INTERNAL_IMPL(orientation, xaml_orientation*, xaml_orientation)
};

#endif // !XAML_UI_CONTROLS_SHARED_STACK_PANEL_HPP
