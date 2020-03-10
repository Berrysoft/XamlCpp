#ifndef XAML_PARSER_HPP
#define XAML_PARSER_HPP

#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <tuple>
#include <typeindex>
#include <variant>
#include <vector>
#include <xaml/xaml_node.hpp>

typedef struct _xmlTextReader xmlTextReader;
typedef xmlTextReader* xmlTextReaderPtr;

typedef struct _xmlParserInputBuffer xmlParserInputBuffer;
typedef xmlParserInputBuffer* xmlParserInputBufferPtr;

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

    class parser
    {
    private:
        meta_context* m_ctx{ nullptr };
        xmlTextReaderPtr m_reader{ nullptr };
        xmlParserInputBufferPtr m_buffer{ nullptr };
        std::set<std::string> m_headers{};

    public:
        constexpr bool is_open() const noexcept { return m_reader; }

        XAML_API void open(std::string_view file);
        XAML_API void load(std::string_view xml);

        std::set<std::string> const& get_headers() const noexcept { return m_headers; }

        struct load_memory_t
        {
        };

        static constexpr load_memory_t load_memory{};

        XAML_API parser(meta_context& ctx);
        parser(meta_context& ctx, std::string_view file) : parser(ctx) { open(file); }
        parser(meta_context& ctx, std::string_view xml, load_memory_t) : parser(ctx) { load(xml); }
        XAML_API ~parser();

    private:
        XAML_API markup_node parse_markup(std::string_view value);
        XAML_API int parse_members(xaml_node& mc);
        XAML_API std::tuple<int, xaml_node> parse_impl();
        XAML_API void clean_up(int ret);

    public:
        XAML_API xaml_node parse();
    };
} // namespace xaml

#endif // !XAML_PARSER_HPP
