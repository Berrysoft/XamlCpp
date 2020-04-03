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
    public:
        constexpr xml_attribute() : xml_base() {}
        constexpr xml_attribute(xml_attribute&& b) : xml_base(std::move(b)) {}
        constexpr xml_attribute& operator=(xml_attribute&& b)
        {
            xml_base::operator=(std::move(b));
            return *this;
        }

        ~xml_attribute() override {}
    };
} // namespace rapidxml

#endif // !RAPID_XML_ATTRIBUTE_HPP
