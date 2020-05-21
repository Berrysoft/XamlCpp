#ifndef XAML_UI_CONTROLS_SHARED_GRID_HPP
#define XAML_UI_CONTROLS_SHARED_GRID_HPP

#include <shared/layout_base.hpp>
#include <xaml/ui/controls/grid.h>

struct xaml_grid_internal : xaml_layout_base_internal
{
    XAML_PROP_PTR_IMPL(columns, xaml_vector)
    XAML_PROP_PTR_IMPL(rows, xaml_vector)

    xaml_result XAML_CALL add_column(xaml_grid_length const& length) noexcept
    {
        xaml_ptr<xaml_object> box;
        XAML_RETURN_IF_FAILED(xaml_box_value(length, &box));
        return m_columns->append(box);
    }

    xaml_result XAML_CALL remove_column([[maybe_unused]] xaml_grid_length const& length) noexcept
    {
        // TODO
        return XAML_E_NOTIMPL;
    }

    xaml_result XAML_CALL add_row(xaml_grid_length const& length) noexcept
    {
        xaml_ptr<xaml_object> box;
        XAML_RETURN_IF_FAILED(xaml_box_value(length, &box));
        return m_rows->append(box);
    }

    xaml_result XAML_CALL remove_row([[maybe_unused]] xaml_grid_length const& length) noexcept
    {
        // TODO
        return XAML_E_NOTIMPL;
    }

    xaml_result XAML_CALL draw_impl(xaml_rectangle const&, std::function<xaml_result(xaml_control*, xaml_rectangle const&)> const&) noexcept override;

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_grid_impl : xaml_layout_base_implement<xaml_grid_impl, xaml_grid_internal, xaml_grid>
{
    XAML_PROP_PTR_INTERNAL_IMPL(columns, xaml_vector)
    XAML_PROP_PTR_INTERNAL_IMPL(rows, xaml_vector)

    XAML_CPROP_INTERNAL_IMPL(column, xaml_grid_length const&, xaml_grid_length const&)
    XAML_CPROP_INTERNAL_IMPL(row, xaml_grid_length const&, xaml_grid_length const&)
};

#endif // !XAML_UI_CONTROLS_SHARED_GRID_HPP
