#ifndef XAML_UI_INTERNAL_SHARED_GRID_HPP
#define XAML_UI_INTERNAL_SHARED_GRID_HPP

#include <tuple>
#include <vector>
#include <xaml/ui/grid.hpp>

namespace xaml
{
    std::vector<std::tuple<double, double>> get_real_length(std::vector<grid_length> const& lengths, std::vector<std::shared_ptr<control>> const& children, double total, bool vertical);
} // namespace xaml

#endif // !XAML_UI_INTERNAL_SHARED_GRID_HPP
