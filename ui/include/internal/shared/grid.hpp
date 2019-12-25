#ifndef XAML_UI_INTERNAL_SHARED_GRID_HPP
#define XAML_UI_INTERNAL_SHARED_GRID_HPP

#include <vector>
#include <xaml/ui/grid.hpp>

namespace xaml
{
    std::vector<double> get_real_length(std::vector<grid_length> const& lengths, double total);
} // namespace xaml

#endif // !XAML_UI_INTERNAL_SHARED_GRID_HPP
