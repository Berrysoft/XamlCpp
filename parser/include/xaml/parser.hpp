#ifndef XAML_PARSER_HPP
#define XAML_PARSER_HPP

#include <filesystem>
#include <set>
#include <string>
#include <string_view>
#include <tuple>
#include <typeindex>
#include <xaml/xaml_node.hpp>

namespace xaml
{
    XAML_API void init_parser(meta_context& ctx) noexcept;

    struct xaml_no_default_constructor : std::logic_error
    {
        XAML_API xaml_no_default_constructor(reflection_info const* t);
        ~xaml_no_default_constructor() override {}
    };

    struct xaml_bad_type : std::logic_error
    {
        XAML_API xaml_bad_type(std::string_view ns, std::string_view name);
        ~xaml_bad_type() override {}
    };

    struct xaml_parse_error : std::logic_error
    {
        xaml_parse_error() : logic_error("Failed to parse XAML file.") {}
        ~xaml_parse_error() override {}
    };

    struct xaml_no_member : std::logic_error
    {
        XAML_API xaml_no_member(reflection_info const* type, std::string_view name);
        ~xaml_no_member() override {}
    };

    XAML_API std::tuple<xaml_node, std::set<std::string>> parse_file(meta_context& ctx, std::filesystem::path const& file);
    XAML_API std::tuple<xaml_node, std::set<std::string>> parse_string(meta_context& ctx, std::string_view xml);
} // namespace xaml

#endif // !XAML_PARSER_HPP
