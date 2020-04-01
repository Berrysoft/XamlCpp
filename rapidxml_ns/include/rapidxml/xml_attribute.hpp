#ifndef RAPID_XML_ATTRIBUTE_HPP
#define RAPID_XML_ATTRIBUTE_HPP

#include <rapidxml/xml_base.hpp>

namespace rapidxml
{
    class xml_document;

    //! Class representing attribute node of XML document.
    //! Each attribute has name and value strings, which are available through name() and value() functions (inherited from xml_base).
    //! Note that after parse, both name and value of attribute will point to interior of source text used for parsing.
    //! Thus, this text must persist in memory for the lifetime of attribute.
    //! \param char chararacter type to use.
    class xml_attribute : public xml_base
    {

        friend class xml_node;

    public:
        ///////////////////////////////////////////////////////////////////////////
        // Related nodes access

        //! Gets document of which attribute is a child.
        //! \return Pointer to document that contains this attribute, or 0 if there is no parent document.
        xml_document* document() const
        {
            if (xml_node* node = this->parent())
            {
                while (node->parent())
                    node = node->parent();
                return node->type() == node_document ? static_cast<xml_document*>(node) : 0;
            }
            else
                return 0;
        }

        //! Gets previous attribute, optionally matching attribute name.
        //! \param name Name of attribute to find, or 0 to return previous attribute regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
        //! \param name_size Size of name, in characters, or 0 to have size calculated automatically from string
        //! \param case_sensitive Should name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
        //! \return Pointer to found attribute, or 0 if not found.
        xml_attribute* previous_attribute(std::optional<std::string_view> name = std::nullopt, bool case_sensitive = true) const
        {
            if (name)
            {
                for (xml_attribute* attribute = m_prev_attribute; attribute; attribute = attribute->m_prev_attribute)
                    if (internal::compare(attribute->name(), *name, case_sensitive))
                        return attribute;
                return nullptr;
            }
            else
                return this->m_parent ? m_prev_attribute : nullptr;
        }

        //! Gets next attribute, optionally matching attribute name.
        //! \param name Name of attribute to find, or 0 to return next attribute regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
        //! \param name_size Size of name, in characters, or 0 to have size calculated automatically from string
        //! \param case_sensitive Should name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
        //! \return Pointer to found attribute, or 0 if not found.
        xml_attribute* next_attribute(std::optional<std::string_view> name = std::nullopt, bool case_sensitive = true) const
        {
            if (name)
            {
                for (xml_attribute* attribute = m_next_attribute; attribute; attribute = attribute->m_next_attribute)
                    if (internal::compare(attribute->name(), *name, case_sensitive))
                        return attribute;
                return nullptr;
            }
            else
                return this->m_parent ? m_next_attribute : nullptr;
        }

        //! Gets next attribute, matching attribute local name and attribute namespace URI .
        //! \param namespace_uri Namespace URI of attribute to find; this string have to be zero-terminated
        //! \param local_name Local name of attribute to find; this string have to be zero-terminated
        //! \param local_name_case_sensitive Should local name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
        //! \return Pointer to found attribute, or 0 if not found.
        xml_attribute* next_attribute_ns(std::string_view namespace_uri, std::string_view local_name, bool local_name_case_sensitive = true) const
        {
            for (xml_attribute* attribute = m_next_attribute; attribute; attribute = attribute->m_next_attribute)
                if (internal::compare(attribute->local_name(),
                                      local_name, local_name_case_sensitive) &&
                    internal::compare(attribute->namespace_uri(),
                                      namespace_uri))
                    return attribute;
            return nullptr;
        }

    private:
        xml_attribute* m_prev_attribute{ nullptr }; // Pointer to previous sibling of attribute, or 0 if none; only valid if parent is non-zero
        xml_attribute* m_next_attribute{ nullptr }; // Pointer to next sibling of attribute, or 0 if none; only valid if parent is non-zero
    };
} // namespace rapidxml

#endif // !RAPID_XML_ATTRIBUTE_HPP
