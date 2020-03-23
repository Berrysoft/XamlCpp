#ifndef XAML_UI_SCREEN_HPP
#define XAML_UI_SCREEN_HPP

#include <vector>
#include <xaml/ui/drawing.hpp>
#include <xaml/utility.hpp>

namespace xaml
{
    struct monitor
    {
        rectangle region;
        rectangle client;
    };

    XAML_UI_API std::vector<monitor> get_monitors();
} // namespace xaml

#endif // !XAML_UI_SCREEN_HPP
