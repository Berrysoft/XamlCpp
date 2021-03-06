#ifndef XAML_UI_CONTROLS_SHARED_LAYOUT_BASE_HPP
#define XAML_UI_CONTROLS_SHARED_LAYOUT_BASE_HPP

#include <shared/container.hpp>
#include <unordered_map>
#include <xaml/ui/controls/layout_base.h>

struct xaml_layout_base_internal : xaml_multicontainer_internal
{
    virtual xaml_result XAML_CALL draw_impl(xaml_rectangle const& region, __xaml_function_view_wrapper_t<xaml_result(xaml_control*, xaml_rectangle const&) noexcept>) noexcept;

    xaml_result XAML_CALL draw(xaml_rectangle const& region) noexcept override;
};

template <typename T, typename Internal, typename Base>
struct xaml_layout_base_implement : xaml_multicontainer_implement<T, Internal, Base>
{
};

#endif // !XAML_UI_CONTROLS_SHARED_LAYOUT_BASE_HPP
