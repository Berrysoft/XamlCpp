#ifndef RAPID_XML_ATTRIBUTE_HPP
#define RAPID_XML_ATTRIBUTE_HPP

#include <optional>
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
        ~xml_attribute() override {}

        ///////////////////////////////////////////////////////////////////////////
        // Related nodes access

        //! Gets document of which attribute is a child.
        //! \return Pointer to document that contains this attribute, or 0 if there is no parent document.
        RAPIDXML_API xml_document* document() const;

        //! Gets previous attribute, optionally matching attribute name.
        //! \param name Name of attribute to find, or 0 to return previous attribute regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
        //! \return Pointer to found attribute, or 0 if not found.
        RAPIDXML_API xml_attribute* previous_attribute(std::optional<std::string_view> name = std::nullopt) const;

        //! Gets next attribute, optionally matching attribute name.
        //! \param name Name of attribute to find, or 0 to return next attribute regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
        //! \return Pointer to found attribute, or 0 if not found.
        RAPIDXML_API xml_attribute* next_attribute(std::optional<std::string_view> name = std::nullopt) const;

        //! Gets next attribute, matching attribute local name and attribute namespace URI .
        //! \param namespace_uri Namespace URI of attribute to find; this string have to be zero-terminated
        //! \param local_name Local name of attribute to find; this string have to be zero-terminated
        //! \return Pointer to found attribute, or 0 if not found.
        RAPIDXML_API xml_attribute* next_attribute_ns(std::string_view namespace_uri, std::string_view local_name) const;

    private:
        xml_attribute* m_prev_attribute{ nullptr }; // Pointer to previous sibling of attribute, or 0 if none; only valid if parent is non-zero
        xml_attribute* m_next_attribute{ nullptr }; // Pointer to next sibling of attribute, or 0 if none; only valid if parent is non-zero
    };
} // namespace rapidxml

#endif // !RAPID_XML_ATTRIBUTE_HPP
