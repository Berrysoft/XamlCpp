#ifndef XAML_UI_SCREEN_HPP
#define XAML_UI_SCREEN_HPP

#include <vector>
#include <xaml/ui/drawing.hpp>
#include <xaml/utility.hpp>

namespace xaml
{
    XAML_API unsigned int screen_dpi();

    struct monitor
    {
        rectangle region;
        rectangle client;
    };

    XAML_API std::vector<monitor> get_monitors();
} // namespace xaml

#endif // !XAML_UI_SCREEN_HPP
