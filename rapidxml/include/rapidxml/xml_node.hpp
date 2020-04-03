#ifndef RAPID_XML_NODE_HPP
#define RAPID_XML_NODE_HPP

#include <list>
#include <optional>
#include <rapidxml/xml_base.hpp>

namespace rapidxml
{
    class xml_attribute;
    class xml_document;

    //! Class representing a node of XML document.
    //! Each node may have associated name and value strings, which are available through name() and value() functions.
    //! Interpretation of name and value depends on type of the node.
    //! Type of node can be determined by using type() function.
    //! <br><br>
    //! Note that after parse, both name and value of node, if any, will point interior of source text used for parsing.
    //! Thus, this text must persist in the memory for the lifetime of node.
    //! \param char chararacter type to use.
    class xml_node : public xml_base
    {
    private:
        ///////////////////////////////////////////////////////////////////////////
        // Data members

        // Note that some of the pointers below have UNDEFINED values if certain other pointers are 0.
        // This is required for maximum performance, as it allows the parser to omit initialization of
        // unneded/redundant values.

        node_type m_type; // Type of node; always valid

        pmr::list<xml_node> m_nodes;
        pmr::list<xml_attribute> m_attributes;

    public:
        ///////////////////////////////////////////////////////////////////////////
        // Construction & destruction

        //! Constructs an empty node with the specified type.
        //! Consider using memory_pool of appropriate document to allocate nodes manually.
        //! \param type Type of node to construct.
        xml_node(node_type type, pmr::polymorphic_allocator<xml_node> const& node_allocator, pmr::polymorphic_allocator<xml_attribute> const& attr_allocator) noexcept
            : xml_base(), m_type(type), m_nodes(node_allocator), m_attributes(attr_allocator) {}

        xml_node(xml_node&& n) : xml_base(std::move(n)), m_type(n.m_type), m_nodes(std::move(n.m_nodes)), m_attributes(std::move(n.m_attributes)) {}
        xml_node& operator=(xml_node&& n)
        {
            xml_base::operator=(std::move(n));
            m_type = n.m_type;
            m_nodes = std::move(n.m_nodes);
            m_attributes = std::move(n.m_attributes);
            return *this;
        }

        ~xml_node() override {}

        ///////////////////////////////////////////////////////////////////////////
        // Node data access

        //! Gets type of node.
        //! \return Type of node.
        constexpr node_type type() const noexcept
        {
            return m_type;
        }

        ///////////////////////////////////////////////////////////////////////////
        // Related nodes access

        pmr::list<xml_node>& nodes() { return m_nodes; }
        pmr::list<xml_node> const& nodes() const { return m_nodes; }

        pmr::list<xml_attribute>& attributes() { return m_attributes; }
        pmr::list<xml_attribute> const& attributes() const { return m_attributes; }

        ///////////////////////////////////////////////////////////////////////////
        // Node modification

        //! Sets type of node.
        //! \param type Type of node to set.
        void type(node_type type)
        {
            m_type = type;
        }
    };
} // namespace rapidxml

#endif // !RAPID_XML_NODE_HPP
