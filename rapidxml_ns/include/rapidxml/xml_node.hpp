#ifndef RAPID_XML_NODE_HPP
#define RAPID_XML_NODE_HPP

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

    public:
        ///////////////////////////////////////////////////////////////////////////
        // Construction & destruction

        //! Constructs an empty node with the specified type.
        //! Consider using memory_pool of appropriate document to allocate nodes manually.
        //! \param type Type of node to construct.
        xml_node(node_type type) : m_type(type)
        {
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

        //! Gets document of which node is a child.
        //! \return Pointer to document that contains this node, or 0 if there is no parent document.
        xml_document* document() const;

        //! Gets first child node, optionally matching node name.
        //! \param name Name of child to find, or 0 to return first child regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
        //! \param name_size Size of name, in characters, or 0 to have size calculated automatically from string
        //! \param case_sensitive Should name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
        //! \return Pointer to found child, or 0 if not found.
        xml_node* first_node(std::optional<std::string_view> name = std::nullopt, bool case_sensitive = true) const;

        //! Gets first child node, matching node local name and namespace URI.
        //! \param namespace_uri Namespace URI of child to find; this string have to be zero-terminated
        //! \param local_name Local name of child to find; this string have to be zero-terminated
        //! \param local_name_case_sensitive Should local name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
        //! \return Pointer to found child, or 0 if not found.
        xml_node* first_node_ns(std::string_view namespace_uri, std::string_view local_name, bool local_name_case_sensitive = true) const;

        xml_node* first_node_ns(std::string_view namespace_uri) const;

        //! Gets last child node, optionally matching node name.
        //! Behaviour is undefined if node has no children.
        //! Use first_node() to test if node has children.
        //! \param name Name of child to find, or 0 to return last child regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
        //! \param name_size Size of name, in characters, or 0 to have size calculated automatically from string
        //! \param case_sensitive Should name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
        //! \return Pointer to found child, or 0 if not found.
        xml_node* last_node(std::optional<std::string_view> name = std::nullopt, bool case_sensitive = true) const;

        //! Gets previous sibling node, optionally matching node name.
        //! Behaviour is undefined if node has no parent.
        //! Use parent() to test if node has a parent.
        //! \param name Name of sibling to find, or 0 to return previous sibling regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
        //! \param name_size Size of name, in characters, or 0 to have size calculated automatically from string
        //! \param case_sensitive Should name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
        //! \return Pointer to found sibling, or 0 if not found.
        xml_node* previous_sibling(std::optional<std::string_view> name = std::nullopt, bool case_sensitive = true) const;

        //! Gets next sibling node, optionally matching node name.
        //! Behaviour is undefined if node has no parent.
        //! Use parent() to test if node has a parent.
        //! \param name Name of sibling to find, or 0 to return next sibling regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
        //! \param name_size Size of name, in characters, or 0 to have size calculated automatically from string
        //! \param case_sensitive Should name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
        //! \return Pointer to found sibling, or 0 if not found.
        xml_node* next_sibling(std::optional<std::string_view> name = std::nullopt, bool case_sensitive = true) const;

        xml_node* next_sibling_ns(std::string_view namespace_uri, std::string_view local_name, bool local_name_case_sensitive = true) const;

        xml_node* next_sibling_ns(std::string_view namespace_uri) const;

        //! Gets first attribute of node, optionally matching attribute name.
        //! \param name Name of attribute to find, or 0 to return first attribute regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
        //! \param name_size Size of name, in characters, or 0 to have size calculated automatically from string
        //! \param case_sensitive Should name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
        //! \return Pointer to found attribute, or 0 if not found.
        xml_attribute* first_attribute(std::optional<std::string_view> name = std::nullopt, bool case_sensitive = true) const;

        //! Gets first attribute of node, matching attribute namespace URI and local name.
        //! \param namespace_uri Namespace URI of attribute to find; this string have to be zero-terminated
        //! \param local_name Local name of attribute to find; this string have to be zero-terminated
        //! \param local_name_case_sensitive Should local name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
        //! \return Pointer to found attribute, or 0 if not found.
        xml_attribute* first_attribute_ns(std::string_view namespace_uri, std::string_view local_name, bool local_name_case_sensitive = true) const;

        //! Gets last attribute of node, optionally matching attribute name.
        //! \param name Name of attribute to find, or 0 to return last attribute regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
        //! \param name_size Size of name, in characters, or 0 to have size calculated automatically from string
        //! \param case_sensitive Should name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
        //! \return Pointer to found attribute, or 0 if not found.
        xml_attribute* last_attribute(std::optional<std::string_view> name = std::nullopt, bool case_sensitive = true) const;

        ///////////////////////////////////////////////////////////////////////////
        // Node modification

        //! Sets type of node.
        //! \param type Type of node to set.
        void type(node_type type)
        {
            m_type = type;
        }

        ///////////////////////////////////////////////////////////////////////////
        // Node manipulation

        //! Prepends a new child node.
        //! The prepended child becomes the first child, and all existing children are moved one position back.
        //! \param child Node to prepend.
        void prepend_node(xml_node* child);

        //! Appends a new child node.
        //! The appended child becomes the last child.
        //! \param child Node to append.
        void append_node(xml_node* child);

        //! Inserts a new child node at specified place inside the node.
        //! All children after and including the specified node are moved one position back.
        //! \param where Place where to insert the child, or 0 to insert at the back.
        //! \param child Node to insert.
        void insert_node(xml_node* where, xml_node* child);

        //! Removes first child node.
        //! If node has no children, behaviour is undefined.
        //! Use first_node() to test if node has children.
        void remove_first_node();

        //! Removes last child of the node.
        //! If node has no children, behaviour is undefined.
        //! Use first_node() to test if node has children.
        void remove_last_node();

        //! Removes specified child from the node
        // \param where Pointer to child to be removed.
        void remove_node(xml_node* where);

        //! Removes all child nodes (but not attributes).
        void remove_all_nodes();

        //! Prepends a new attribute to the node.
        //! \param attribute Attribute to prepend.
        void prepend_attribute(xml_attribute* attribute);

        //! Appends a new attribute to the node.
        //! \param attribute Attribute to append.
        void append_attribute(xml_attribute* attribute);

        //! Inserts a new attribute at specified place inside the node.
        //! All attributes after and including the specified attribute are moved one position back.
        //! \param where Place where to insert the attribute, or 0 to insert at the back.
        //! \param attribute Attribute to insert.
        void insert_attribute(xml_attribute* where, xml_attribute* attribute);

        //! Removes first attribute of the node.
        //! If node has no attributes, behaviour is undefined.
        //! Use first_attribute() to test if node has attributes.
        void remove_first_attribute();

        //! Removes last attribute of the node.
        //! If node has no attributes, behaviour is undefined.
        //! Use first_attribute() to test if node has attributes.
        void remove_last_attribute();

        //! Removes specified attribute from node.
        //! \param where Pointer to attribute to be removed.
        void remove_attribute(xml_attribute* where);

        //! Removes all attributes of node.
        void remove_all_attributes();

        ///////////////////////////////////////////////////////////////////////////
        // Restrictions

        // No copying
        xml_node(const xml_node&) = delete;
        void operator=(const xml_node&) = delete;

    private:
        ///////////////////////////////////////////////////////////////////////////
        // Data members

        // Note that some of the pointers below have UNDEFINED values if certain other pointers are 0.
        // This is required for maximum performance, as it allows the parser to omit initialization of
        // unneded/redundant values.
        //
        // The rules are as follows:
        // 1. first_node and first_attribute contain valid pointers, or 0 if node has no children/attributes respectively
        // 2. last_node and last_attribute are valid only if node has at least one child/attribute respectively, otherwise they contain garbage
        // 3. prev_sibling and next_sibling are valid only if node has a parent, otherwise they contain garbage

        node_type m_type; // Type of node; always valid
        xml_node* m_first_node{ nullptr }; // Pointer to first child node, or 0 if none; always valid
        xml_node* m_last_node{ nullptr }; // Pointer to last child node, or 0 if none; this value is only valid if m_first_node is non-zero
        xml_attribute* m_first_attribute{ nullptr }; // Pointer to first attribute of node, or 0 if none; always valid
        xml_attribute* m_last_attribute{ nullptr }; // Pointer to last attribute of node, or 0 if none; this value is only valid if m_first_attribute is non-zero
        xml_node* m_prev_sibling{ nullptr }; // Pointer to previous sibling of node, or 0 if none; this value is only valid if m_parent is non-zero
        xml_node* m_next_sibling{ nullptr }; // Pointer to next sibling of node, or 0 if none; this value is only valid if m_parent is non-zero
    };
} // namespace rapidxml

#endif // !RAPID_XML_NODE_HPP
