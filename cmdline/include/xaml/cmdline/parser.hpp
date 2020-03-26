#ifndef XAML_CMDLINE_PARSER_HPP
#define XAML_CMDLINE_PARSER_HPP

#include <map>
#include <stdexcept>
#include <vector>
#include <xaml/array_view.hpp>
#include <xaml/meta/meta.hpp>
#include <xaml/strings.hpp>

namespace xaml::cmdline
{
    XAML_CMDLINE_API void init_parser(xaml::meta_context& ctx) noexcept;

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

    struct option_property
    {
        xaml::property_info const* info;
        xaml::string_t value;
    };

    struct option_collection_property
    {
        xaml::collection_property_info const* info;
        std::vector<xaml::string_t> values;
    };

    struct options
    {
        std::vector<option_property> properties;
        std::map<std::string, option_collection_property> collection_properties;
    };

    XAML_CMDLINE_API options parse(xaml::reflection_info const* refl, xaml::array_view<xaml::string_t> args);

    inline options parse(xaml::reflection_info const* refl, int argc, xaml::char_t const* const* argv)
    {
        if (argc > 1)
        {
            std::vector<xaml::string_t> args{ argv + 1, argv + argc };
            return parse(refl, args);
        }
        return {};
    }
} // namespace xaml::cmdline

#endif // !XAML_CMDLINE_PARSER_HPP
