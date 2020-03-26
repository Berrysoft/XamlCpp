#ifndef XAML_CMDLINE_PARSER_HPP
#define XAML_CMDLINE_PARSER_HPP

#include <stdexcept>
#include <vector>
#include <xaml/array_view.hpp>
#include <xaml/meta/meta.hpp>
#include <xaml/strings.hpp>

namespace xaml::cmdline
{
    struct invalid_option : std::logic_error
    {
        XAML_CMDLINE_API invalid_option(string_view_t opt);
        ~invalid_option() override {}
    };

    struct option_node
    {
        std::string option;
        property_info const* info;
        std::string value;
    };

    XAML_CMDLINE_API std::vector<option_node> parse(reflection_info const* refl, array_view<string_t> args);

    inline std::vector<option_node> parse(reflection_info const* refl, int argc, char_t const* const* argv)
    {
        std::vector<string_t> args{ argv, argv + argc };
        return parse(refl, args);
    }
} // namespace xaml::cmdline

#endif // !XAML_CMDLINE_PARSER_HPP
