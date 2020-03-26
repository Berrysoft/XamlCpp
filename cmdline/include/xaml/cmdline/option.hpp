#ifndef XAML_CMDLINE_OPTION_HPP
#define XAML_CMDLINE_OPTION_HPP

#include <string>
#include <xaml/meta/meta.hpp>

namespace xaml::cmdline
{
    struct option
    {
        std::string option;
        property_info const* info;
        std::string value;
    };
} // namespace xaml::cmdline

#endif // !XAML_CMDLINE_OPTION_HPP
