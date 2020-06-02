#ifndef RAPID_XML_DOCUMENT_HPP
#define RAPID_XML_DOCUMENT_HPP

#include <optional>
#include <rapidxml/xml_node.hpp>
#include <vector>

#ifndef __cpp_lib_memory_resource
    #include <boost/container/pmr/monotonic_buffer_resource.hpp>
#endif // !__cpp_lib_memory_resource

namespace rapidxml
{
    struct xml_namespace
    {
        static constexpr std::string_view prefix{ "xml" };

        static constexpr std::string_view uri{ "http://www.w3.org/XML/1998/namespace" };
    };

    struct xmlns_namespace
    {
        static constexpr std::string_view prefix{ "xmlns" };

        static constexpr std::string_view uri{ "http://www.w3.org/2000/xmlns/" };
    };

    ///////////////////////////////////////////////////////////////////////
    // Parsing flags

    enum class parse_flag
    {
        //! Parse flag instructing the parser to not create data nodes.
        //! Text of first data node will still be placed in value of parent element, unless rapidxml_ns::parse_no_element_values flag is also specified.
        //! Can be combined with other flags by use of | operator.
        //! <br><br>
        //! See xml_document::parse() function.
        no_data_nodes = 0x1,

        //! Parse flag instructing the parser to not use text of first data node as a value of parent element.
        //! Can be combined with other flags by use of | operator.
        //! Note that child data nodes of element node take precendence over its value when printing.
        //! That is, if element has one or more child data nodes <em>and</em> a value, the value will be ignored.
        //! Use rapidxml_ns::parse_no_data_nodes flag to prevent creation of data nodes if you want to manipulate data using values of elements.
        //! <br><br>
        //! See xml_document::parse() function.
        no_element_values = 0x2,

        //! Parse flag instructing the parser to create XML declaration node.
        //! By default, declaration node is not created.
        //! Can be combined with other flags by use of | operator.
        //! <br><br>
        //! See xml_document::parse() function.
        declaration_node = 0x20,

        //! Parse flag instructing the parser to create comments nodes.
        //! By default, comment nodes are not created.
        //! Can be combined with other flags by use of | operator.
        //! <br><br>
        //! See xml_document::parse() function.
        comment_nodes = 0x40,

        //! Parse flag instructing the parser to create DOCTYPE node.
        //! By default, doctype node is not created.
        //! Although W3C specification allows at most one DOCTYPE node, RapidXml will silently accept documents with more than one.
        //! Can be combined with other flags by use of | operator.
        //! <br><br>
        //! See xml_document::parse() function.
        doctype_node = 0x80,

        //! Parse flag instructing the parser to create PI nodes.
        //! By default, PI nodes are not created.
        //! Can be combined with other flags by use of | operator.
        //! <br><br>
        //! See xml_document::parse() function.
        pi_nodes = 0x100,

        //! Parse flag instructing the parser to validate closing tag names.
        //! If not set, name inside closing tag is irrelevant to the parser.
        //! By default, closing tags are not validated.
        //! Can be combined with other flags by use of | operator.
        //! <br><br>
        //! See xml_document::parse() function.
        validate_closing_tags = 0x200,

        //! Parse flag instructing the parser to trim all leading and trailing whitespace of data nodes.
        //! By default, whitespace is not trimmed.
        //! This flag does not cause the parser to modify source text.
        //! Can be combined with other flags by use of | operator.
        //! <br><br>
        //! See xml_document::parse() function.
        trim_whitespace = 0x400,

        //! Parse flag instructing the parser to condense all whitespace runs of data nodes to a single space character.
        //! Trimming of leading and trailing whitespace of data is controlled by rapidxml_ns::parse_trim_whitespace flag.
        //! By default, whitespace is not normalized.
        //! If this flag is specified, source text will be modified.
        //! Can be combined with other flags by use of | operator.
        //! <br><br>
        //! See xml_document::parse() function.
        normalize_whitespace = 0x800,

        // Compound flags

        //! Parse flags which represent default behaviour of the parser.
        //! This is always equal to 0, so that all other flags can be simply ored together.
        //! Normally there is no need to inconveniently disable flags by anding with their negated (~) values.
        //! This also means that meaning of each flag is a <i>negation</i> of the default setting.
        //! For example, if flag name is rapidxml_ns::parse_no_utf8, it means that utf-8 is <i>enabled</i> by default,
        //! and using the flag will disable it.
        //! <br><br>
        //! See xml_document::parse() function.
        default_flag = 0,

        //! A combination of parse flags resulting in fastest possible parsing, without sacrificing important data.
        //! <br><br>
        //! See xml_document::parse() function.
        fastest = no_data_nodes,

        //! A combination of parse flags resulting in largest amount of data being extracted.
        //! This usually results in slowest parsing.
        //! <br><br>
        //! See xml_document::parse() function.
        full = declaration_node | comment_nodes | doctype_node | pi_nodes | validate_closing_tags
    };

    constexpr parse_flag operator&(parse_flag lhs, parse_flag rhs) { return (parse_flag)((int)lhs & (int)rhs); }
    constexpr parse_flag operator|(parse_flag lhs, parse_flag rhs) { return (parse_flag)((int)lhs | (int)rhs); }
    constexpr parse_flag operator^(parse_flag lhs, parse_flag rhs) { return (parse_flag)((int)lhs ^ (int)rhs); }
    constexpr parse_flag& operator&=(parse_flag& lhs, parse_flag rhs) { return lhs = lhs & rhs; }
    constexpr parse_flag& operator|=(parse_flag& lhs, parse_flag rhs) { return lhs = lhs | rhs; }
    constexpr parse_flag& operator^=(parse_flag& lhs, parse_flag rhs) { return lhs = lhs ^ rhs; }
    constexpr parse_flag operator~(parse_flag f) { return (parse_flag)(~(int)f); }

    //! This class represents root of the DOM hierarchy.
    //! It is also an xml_node and a memory_pool through public inheritance.
    //! Use parse() function to build a DOM tree from a zero-terminated XML text string.
    //! parse() function allocates memory for nodes and attributes by using functions of xml_document,
    //! which are inherited from memory_pool.
    //! To access root node of the document, use the document itself, as if it was an xml_node.
    //! \param char chararacter type to use.
    class xml_document
    {
    private:
        // Size of static memory block.
        static constexpr int32_t static_pool_size = 64 * 1024;

        pmr::monotonic_buffer_resource m_pool{ static_pool_size };

        pmr::polymorphic_allocator<char> m_buffer_allocator{ &m_pool };

        pmr::string m_buffer{ m_buffer_allocator };

        pmr::polymorphic_allocator<xml_node> m_node_allocator{ &m_pool };
        pmr::polymorphic_allocator<xml_attribute> m_attribute_allocator{ &m_pool };

        xml_node m_root_node{ node_type::document, m_node_allocator, m_attribute_allocator };

    public:
        constexpr xml_node& node() noexcept { return m_root_node; }
        constexpr xml_node const& node() const noexcept { return m_root_node; }

        template <typename T>
        pmr::polymorphic_allocator<T> get_allocator() const
        {
            return pmr::polymorphic_allocator<T>{ &m_pool };
        }

        RAPIDXML_API void load_string(std::string_view str, parse_flag flags = parse_flag::default_flag);
        RAPIDXML_API void load_stream(std::istream& stream, parse_flag flags = parse_flag::default_flag);

    private:
        //! Parses zero-terminated XML string according to given flags.
        //! The string must persist for the lifetime of the document.
        //! In case of error, rapidxml::parse_error exception will be thrown.
        //! <br><br>
        //! If you want to parse contents of a file, you must first load the file into the memory, and pass pointer to its beginning.
        //! Make sure that data is zero-terminated.
        //! <br><br>
        //! Document can be parsed into multiple times.
        //! Each new call to parse removes previous nodes and attributes (if any), but does not clear memory pool.
        //! \param text XML data to parse; pointer is non-const to denote fact that this data may be modified by the parser.
        RAPIDXML_API void parse(char* text, parse_flag flags);
    };
} // namespace rapidxml

#endif // !RAPID_XML_DOCUMENT_HPP
