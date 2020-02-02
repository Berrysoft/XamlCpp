#ifndef XAML_UI_INTERNAL_SHARED_GRID_HPP
#define XAML_UI_INTERNAL_SHARED_GRID_HPP

#include <tuple>
#include <vector>
#include <xaml/ui/controls/grid.hpp>

namespace xaml
{
    std::vector<std::tuple<double, double>> get_real_length(array_view<grid_length> lengths, array_view<std::shared_ptr<control>> children, double total, bool vertical);
    rectangle get_real_region(std::shared_ptr<control> c, rectangle max_region);
} // namespace xaml

#endif // !XAML_UI_INTERNAL_SHARED_GRID_HPP
