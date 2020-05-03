#ifndef XAML_UI_CONTROLS_SHARED_GRID_HPP
#define XAML_UI_CONTROLS_SHARED_GRID_HPP

#include <shared/layout_base.hpp>
#include <xaml/ui/controls/grid.h>

struct xaml_grid_internal : xaml_layout_base_internal
{
protected:
    xaml_ptr<xaml_vector> m_columns;
    xaml_ptr<xaml_vector> m_rows;

public:
    xaml_result XAML_CALL get_columns(xaml_vector_view** ptr) noexcept { return m_columns->query(ptr); }
    xaml_result XAML_CALL get_rows(xaml_vector_view** ptr) noexcept { return m_rows->query(ptr); }

    xaml_result XAML_CALL add_column(xaml_grid_length const& length) noexcept
    {
        xaml_ptr<xaml_object> box;
        XAML_RETURN_IF_FAILED(xaml_box_value(length, &box));
        return m_columns->append(box.get());
    }

    xaml_result XAML_CALL remove_column(xaml_grid_length const& length) noexcept { return XAML_E_NOTIMPL; }

    xaml_result XAML_CALL add_row(xaml_grid_length const& length) noexcept
    {
        xaml_ptr<xaml_object> box;
        XAML_RETURN_IF_FAILED(xaml_box_value(length, &box));
        return m_rows->append(box.get());
    }

    xaml_result XAML_CALL remove_row(xaml_grid_length const& length) noexcept { return XAML_E_NOTIMPL; }

    xaml_result XAML_CALL draw_impl(xaml_rectangle const&, std::function<xaml_result(xaml_control*, xaml_rectangle const&)> const&) noexcept override;

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_grid_impl : xaml_layout_base_implement<xaml_grid_impl, xaml_grid_internal, xaml_grid>
{
    XAML_PROP_INTERNAL_IMPL_BASE(columns, xaml_vector_view**)
    XAML_PROP_INTERNAL_IMPL_BASE(rows, xaml_vector_view**)

    XAML_CPROP_INTERNAL_IMPL(column, xaml_grid_length const&, xaml_grid_length const&)
    XAML_CPROP_INTERNAL_IMPL(row, xaml_grid_length const&, xaml_grid_length const&)
};

#endif // !XAML_UI_CONTROLS_SHARED_GRID_HPP
