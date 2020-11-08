#ifndef XAML_UI_CONTROLS_SHARED_GRID_HPP
#define XAML_UI_CONTROLS_SHARED_GRID_HPP

#include <shared/layout_base.hpp>
#include <xaml/ui/controls/grid.h>

struct xaml_grid_internal : xaml_layout_base_internal
{
    XAML_PROP_PTR_IMPL(columns, xaml_vector<xaml_grid_length>)
    XAML_PROP_PTR_IMPL(rows, xaml_vector<xaml_grid_length>)

    xaml_result XAML_CALL add_column(xaml_grid_length const& length) noexcept
    {
        return m_columns->append(length);
    }

    xaml_result XAML_CALL remove_column(xaml_grid_length const& length) noexcept
    {
        std::int32_t index;
        XAML_RETURN_IF_FAILED(m_columns->index_of(length, &index));
        if (index != -1)
        {
            XAML_RETURN_IF_FAILED(m_columns->remove_at(index));
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL add_row(xaml_grid_length const& length) noexcept
    {
        return m_rows->append(length);
    }

    xaml_result XAML_CALL remove_row(xaml_grid_length const& length) noexcept
    {
        std::int32_t index;
        XAML_RETURN_IF_FAILED(m_rows->index_of(length, &index));
        if (index != -1)
        {
            XAML_RETURN_IF_FAILED(m_rows->remove_at(index));
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL draw_impl(xaml_rectangle const&, __xaml_function_view_wrapper_t<xaml_result(xaml_control*, xaml_rectangle const&) noexcept>) noexcept override;

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_grid_impl : xaml_layout_base_implement<xaml_grid_impl, xaml_grid_internal, xaml_grid>
{
    XAML_PROP_PTR_INTERNAL_IMPL(columns, xaml_vector<xaml_grid_length>)
    XAML_PROP_PTR_INTERNAL_IMPL(rows, xaml_vector<xaml_grid_length>)

    XAML_CPROP_INTERNAL_IMPL(column, xaml_grid_length const&, xaml_grid_length const&)
    XAML_CPROP_INTERNAL_IMPL(row, xaml_grid_length const&, xaml_grid_length const&)
};

#endif // !XAML_UI_CONTROLS_SHARED_GRID_HPP
