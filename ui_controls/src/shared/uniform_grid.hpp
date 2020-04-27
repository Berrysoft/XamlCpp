#ifndef XAML_UI_CONTROLS_SHARED_UNIFORM_GRID_HPP
#define XAML_UI_CONTROLS_SHARED_UNIFORM_GRID_HPP

#include <shared/layout_base.hpp>
#include <xaml/ui/controls/uniform_grid.h>

struct xaml_uniform_grid_impl : xaml_layout_base_implement<xaml_uniform_grid_impl, xaml_uniform_grid>
{
    XAML_PROP_IMPL(columns, std::int32_t, std::int32_t*, std::int32_t)
    XAML_PROP_IMPL(rows, std::int32_t, std::int32_t*, std::int32_t)
    XAML_PROP_IMPL(orientation, xaml_orientation, xaml_orientation*, xaml_orientation)

    xaml_result XAML_CALL draw_impl(xaml_rectangle const&, std::function<xaml_result(xaml_control*, xaml_rectangle const&)> const&) noexcept override;
    xaml_result XAML_CALL size_to_fit() noexcept override;
};

#endif // !XAML_UI_CONTROLS_SHARED_UNIFORM_GRID_HPP
