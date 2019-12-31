#ifndef XAML_PARSER_HPP
#define XAML_PARSER_HPP

#include <libxml/xmlreader.h>
#include <string>
#include <string_view>
#include <tuple>
#include <typeindex>
#include <vector>
#include <xaml/meta/meta.hpp>

namespace xaml
{
    struct __xaml_construct_property;

    struct __xaml_property
    {
        property_info info;
        std::string value;
    };

    struct __xaml_event
    {
        event_info info;
        std::string value;
    };

    struct __xaml_node
    {
        std::type_index type;
        std::vector<__xaml_property> properties;
        std::vector<__xaml_event> events;
        std::vector<__xaml_construct_property> construct_properties;
        std::vector<__xaml_node> children;
    };

    struct __xaml_construct_property
    {
        property_info info;
        __xaml_node value;
    };

    struct xaml_no_default_constructor : std::logic_error
    {
        xaml_no_default_constructor(std::type_index t);
        ~xaml_no_default_constructor() override {}
    };

    struct xaml_bad_type : std::logic_error
    {
        xaml_bad_type(std::string_view ns, std::string_view name);
        ~xaml_bad_type() override {}
    };

    struct xaml_parse_error : std::logic_error
    {
        xaml_parse_error() : logic_error("Failed to parse XAML file.") {}
        ~xaml_parse_error() override {}
    };

    struct xaml_no_member : std::logic_error
    {
        xaml_no_member(std::type_index type, std::string_view name);
        ~xaml_no_member() override {}
    };

    struct xaml_not_multicontainer : std::logic_error
    {
        xaml_not_multicontainer(std::type_index type);
        ~xaml_not_multicontainer() override {}
    };

    class parser
    {
    private:
        xmlTextReaderPtr reader;

    public:
        constexpr bool is_open() const noexcept { return reader; }
        void open(std::string_view file);

        parser();
        parser(std::string_view file) : parser() { open(file); }
        ~parser();

    private:
        int parse_members(__xaml_node& mc);
        std::tuple<int, __xaml_node> deserialize_impl();
        void clean_up(int ret);

    public:
        __xaml_node parse();
    };
} // namespace xaml

#endif // !XAML_PARSER_HPP
