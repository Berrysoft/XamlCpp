#ifndef XAML_PARSER_HPP
#define XAML_PARSER_HPP

#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <typeindex>
#include <variant>
#include <vector>
#include <xaml/markup/binding.hpp>
#include <xaml/meta/meta.hpp>
#include <xaml/ui/meta.hpp>
#include <xaml/xaml_node.hpp>

#ifndef XAML_UI_WINRT
#include <libxml/xmlreader.h>
#else
#include "winrt/Windows.Data.Xml.Dom.h"
#endif // !XAML_UI_WINRT

namespace xaml
{
    inline void init_parser() noexcept
    {
        init_context();
        add_module<ui_init_traits>();
        register_class<binding>();
        REGISTER_ENUM(xaml, binding_mode);
    }

    struct xaml_no_default_constructor : std::logic_error
    {
        XAML_API xaml_no_default_constructor(std::type_index t);
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
        XAML_API xaml_no_member(std::type_index type, std::string_view name);
        ~xaml_no_member() override {}
    };

    class parser
    {
    private:
#ifndef XAML_UI_WINRT
        xmlTextReaderPtr reader{ nullptr };
        xmlParserInputBufferPtr buffer{ nullptr };
#else
        winrt::Windows::Data::Xml::Dom::XmlDocument doc{};
        bool opened{ false };
#endif // !XAML_UI_WINRT

    public:
#ifndef XAML_UI_WINRT
        constexpr bool is_open() const noexcept
        {
            return reader;
        }
#else
        constexpr bool is_open() const noexcept
        {
            return opened;
        }
#endif // !XAML_UI_WINRT

        XAML_API void open(std::string_view file);
        XAML_API void load(std::string_view xml);

        struct load_memory_t
        {
        };

        static constexpr load_memory_t load_memory{};

        XAML_API parser();
        parser(std::string_view file) : parser() { open(file); }
        parser(std::string_view xml, load_memory_t) : parser() { load(xml); }
        XAML_API ~parser();

    private:
#ifndef XAML_UI_WINRT
        XAML_API markup_node parse_markup(std::string_view value);
        XAML_API int parse_members(xaml_node& mc);
        XAML_API std::tuple<int, xaml_node> parse_impl();
        XAML_API void clean_up(int ret);
#else
        XAML_API markup_node parse_markup(std::wstring_view value);
        XAML_API void parse_members(xaml_node& mc, winrt::Windows::Data::Xml::Dom::IXmlNode const& e);
        XAML_API xaml_node parse_impl(winrt::Windows::Data::Xml::Dom::IXmlNode const& e);
#endif // !XAML_UI_WINRT

    public:
        XAML_API xaml_node parse();
    };
} // namespace xaml

#endif // !XAML_PARSER_HPP
