#ifndef RAPID_XML_BASE_HPP
#define RAPID_XML_BASE_HPP

#include <list>
#include <rapidxml/utility.hpp>
#include <string_view>
#include <version>

#ifdef __cpp_lib_memory_resource
#include <memory_resource>
#elif __has_include(<experimental/memory_resource>)
#include <experimental/memory_resource>
#else
#error Cannot find <memory_resource>
#endif // __cpp_lib_memory_resource

namespace rapidxml
{
#ifdef __cpp_lib_memory_resource
    namespace pmr = std::pmr;
#else
    namespace pmr
    {
        using namespace std::experimental::pmr;

        template <typename T>
        using list = std::list<T, polymorphic_allocator<T>>;
    } // namespace pmr
#endif // __cpp_lib_memory_resource

    //! Parse error exception.
    //! This exception is thrown by the parser when an error occurs.
    //! Use what() function to get human-readable error message.
    //! Use where() function to get a pointer to position within source text where error was detected.
    //! <br><br>
    //! This will cause the parser to call rapidxml_ns::parse_error_handler() function instead of throwing an exception.
    //! This function must be defined by the user.
    //! <br><br>
    //! This class derives from <code>std::exception</code> class.
    class parse_error : public std::exception
    {

    public:
        //! Constructs parse error
        parse_error(const char* what, const char* where)
            : m_what(what), m_where(where)
        {
        }

        //! Gets human readable description of error.
        //! \return Pointer to null terminated description of the error.
        virtual const char* what() const throw()
        {
            return m_what;
        }

        //! Gets pointer to character data where error happened.
        //! char should be the same as char type of xml_document that produced the error.
        //! \return Pointer to location within the parsed string where error occured.
        const char* where() const
        {
            return m_where;
        }

    private:
        const char* m_what;
        const char* m_where;
    };

    //! Enumeration listing all node types produced by the parser.
    //! Use xml_node::type() function to query node type.
    enum class node_type
    {
        document, //!< A document node. Name and value are empty.
        element, //!< An element node. Name contains element name. Value contains text of first data node.
        data, //!< A data node. Name is empty. Value contains data text.
        cdata, //!< A CDATA node. Name is empty. Value contains data text.
        comment, //!< A comment node. Name is empty. Value contains comment text.
        declaration, //!< A declaration node. Name and value are empty. Declaration parameters (version, encoding and standalone) are in node attributes.
        doctype, //!< A DOCTYPE node. Name is empty. Value contains DOCTYPE text.
        pi //!< A PI node. Name contains target. Value contains instructions.
    };

    class xml_node;

    //! Base class for xml_node and xml_attribute implementing common functions:
    //! name(), name_size(), value(), value_size() and parent().
    //! \param char chararacter type to use
    class xml_base
    {
    protected:
        std::size_t m_local_name{ 0 }; // Pointer into m_name where local part begins
        std::string_view m_name{}; // Name of node, or 0 if no name
        std::string_view m_value{}; // Value of node, or 0 if no value
        std::string_view m_namespace_uri{};
        xml_node* m_parent{ nullptr }; // Pointer to parent node, or 0 if none

    public:
        constexpr xml_base() noexcept {}
        xml_base(xml_base const&) = delete;
        xml_base& operator=(xml_base const&) = delete;

        constexpr xml_base(xml_base&& b)
            : m_local_name(b.m_local_name), m_name(b.m_name), m_value(b.m_value), m_namespace_uri(b.m_namespace_uri), m_parent(b.m_parent) {}
        constexpr xml_base& operator=(xml_base&& b)
        {
            m_local_name = b.m_local_name;
            m_name = b.m_name;
            m_value = b.m_value;
            m_namespace_uri = b.m_namespace_uri;
            m_parent = b.m_parent;
            return *this;
        }

        virtual ~xml_base() {}

        ///////////////////////////////////////////////////////////////////////////
        // Node data access

        constexpr std::string_view local_name() const noexcept { return m_name.substr(m_local_name); }

        constexpr std::size_t local_offset() const noexcept { return m_local_name; }

        //! Gets namespace prefix.
        //! Returned string is never zero-terminated, regardless of parse_no_string_terminators. Use prefix_size()
        //! "Note that the prefix functions only as a placeholder for a namespace name. Applications
        //! SHOULD use the namespace name, not the prefix, in constructing names whose scope extends beyond the containing
        //! document" Namespaces in XML 1.0 (Third Edition)
        constexpr std::string_view prefix() const noexcept { return m_local_name > 1 ? m_name.substr(0, m_local_name - 1) : std::string_view{}; }

        //! Gets QName of the node.
        //! Interpretation of name depends on type of node.
        //! <br><br>
        //! \return Name of node, or empty string if node has no name.
        constexpr std::string_view name() const noexcept { return m_name; }

        //! Gets value of node.
        //! Interpretation of value depends on type of node.
        //! <br><br>
        //! \return Value of node, or empty string if node has no value.
        constexpr std::string_view value() const noexcept { return m_value; }

        //! Gets namespace URI of the node.
        //! <br><br>
        //! \return Namespace URI of node, or empty string if node has no namespace assigned.
        constexpr std::string_view namespace_uri() const noexcept { return m_namespace_uri; }

        ///////////////////////////////////////////////////////////////////////////
        // Node modification

        //! Sets QName of node to a non zero-terminated string.
        //! See \ref ownership_of_strings.
        //! <br><br>
        //! Note that node does not own its name or value, it only stores a pointer to it.
        //! It will not delete or otherwise free the pointer on destruction.
        //! It is reponsibility of the user to properly manage lifetime of the string.
        //! The easiest way to achieve it is to use memory_pool of the document to allocate the string -
        //! on destruction of the document the string will be automatically freed.
        //! <br><br>
        //! Note that passed string will not be automatically divided to prefix and local_name,
        //! it seems useless for manual node creation. qname() may be used instead
        //! local_name() will be set to be equal to name()
        //! <br><br>
        //! \param name QName of node to set. Does not have to be zero terminated.
        void name(std::string_view name) noexcept
        {
            qname(name);
        }

        //! Sets QName as PrefixedName or UnprefixedName where local_part points in QName string
        void qname(std::string_view qname, std::size_t local_part) noexcept
        {
            m_name = qname;
            m_local_name = local_part;
        }

        //! Sets QName as UnprefixedName
        void qname(std::string_view qname) noexcept
        {
            m_name = qname;
            m_local_name = 0;
        }

        void namespace_uri(std::string_view uri) noexcept
        {
            m_namespace_uri = uri;
        }

        //! Sets value of node to a non zero-terminated string.
        //! See \ref ownership_of_strings.
        //! <br><br>
        //! Note that node does not own its name or value, it only stores a pointer to it.
        //! It will not delete or otherwise free the pointer on destruction.
        //! It is responsibility of the user to properly manage lifetime of the string.
        //! The easiest way to achieve it is to use memory_pool of the document to allocate the string -
        //! on destruction of the document the string will be automatically freed.
        //! <br><br>
        //! If an element has a child node of type node_data, it will take precedence over element value when printing.
        //! If you want to manipulate data of elements using values, use parser flag rapidxml_ns::parse_no_data_nodes to prevent creation of data nodes by the parser.
        //! \param value value of node to set. Does not have to be zero terminated.
        void value(std::string_view value) noexcept
        {
            m_value = value;
        }

        ///////////////////////////////////////////////////////////////////////////
        // Related nodes access

        //! Gets node parent.
        //! \return Pointer to parent node, or 0 if there is no parent.
        constexpr xml_node* parent() const noexcept
        {
            return m_parent;
        }

        
        //! Gets root node.
        //! \return Pointer to document that contains this node, or 0 if there is no parent document.
        RAPIDXML_API xml_node* document() const;
    };
} // namespace rapidxml

#endif // !RAPID_XML_BASE_HPP
