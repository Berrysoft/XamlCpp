#ifndef XAML_CMDLINE_PARSER_HPP
#define XAML_CMDLINE_PARSER_HPP

#include <stdexcept>
#include <vector>
#include <xaml/array_view.hpp>
#include <xaml/meta/meta.hpp>
#include <xaml/strings.hpp>

namespace xaml::cmdline
{
    struct no_registered_option : std::logic_error
    {
        XAML_CMDLINE_API no_registered_option();
        ~no_registered_option() override {}
    };

    struct invalid_option : std::logic_error
    {
        XAML_CMDLINE_API invalid_option(string_view_t opt);
        ~invalid_option() override {}
    };

    struct option_node
    {
        xaml::property_info const* info;
        xaml::string_t value;
    };

    XAML_CMDLINE_API std::vector<option_node> parse(xaml::reflection_info const* refl, xaml::array_view<xaml::string_t> args);

    inline std::vector<option_node> parse(xaml::reflection_info const* refl, int argc, xaml::char_t const* const* argv)
    {
        std::vector<xaml::string_t> args{ argv, argv + argc };
        return parse(refl, args);
    }
} // namespace xaml::cmdline

#endif // !XAML_CMDLINE_PARSER_HPP
