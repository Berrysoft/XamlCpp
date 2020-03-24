#ifndef XAML_PARSER_HPP
#define XAML_PARSER_HPP

#include <filesystem>
#include <memory>
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

    struct parser_impl;

    class parser
    {
    private:
        meta_context* m_ctx{ nullptr };
        std::unique_ptr<parser_impl> m_impl{ nullptr };

    public:
        XAML_API bool is_open() const noexcept;

        XAML_API void open(std::filesystem::path const& file);
        XAML_API void load(std::string_view xml);

       XAML_API std::set<std::string> const& get_headers() const noexcept;

        struct load_memory_t
        {
        };

        static constexpr load_memory_t load_memory{};

        XAML_API parser(meta_context& ctx);
        parser(meta_context& ctx, std::filesystem::path const& file) : parser(ctx) { open(file); }
        parser(meta_context& ctx, std::string_view xml, load_memory_t) : parser(ctx) { load(xml); }
        XAML_API ~parser();

    public:
        XAML_API xaml_node parse();
    };
} // namespace xaml

#endif // !XAML_PARSER_HPP
