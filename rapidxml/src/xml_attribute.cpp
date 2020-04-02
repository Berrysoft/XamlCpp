#include <rapidxml/xml_attribute.hpp>
#include <rapidxml/xml_document.hpp>

using namespace std;

namespace rapidxml
{
    xml_document* xml_attribute::document() const
    {
        if (xml_node* node = this->parent())
        {
            while (node->parent())
                node = node->parent();
            return node->type() == node_type::document ? static_cast<xml_document*>(node) : nullptr;
        }
        else
            return nullptr;
    }

    xml_attribute* xml_attribute::previous_attribute(optional<string_view> name) const
    {
        if (name)
        {
            for (xml_attribute* attribute = m_prev_attribute; attribute; attribute = attribute->m_prev_attribute)
                if (attribute->name() == *name)
                    return attribute;
            return nullptr;
        }
        else
            return this->m_parent ? m_prev_attribute : nullptr;
    }

    xml_attribute* xml_attribute::next_attribute(optional<string_view> name) const
    {
        if (name)
        {
            for (xml_attribute* attribute = m_next_attribute; attribute; attribute = attribute->m_next_attribute)
                if (attribute->name() == *name)
                    return attribute;
            return nullptr;
        }
        else
            return this->m_parent ? m_next_attribute : nullptr;
    }

    xml_attribute* xml_attribute::next_attribute_ns(string_view namespace_uri, string_view local_name) const
    {
        for (xml_attribute* attribute = m_next_attribute; attribute; attribute = attribute->m_next_attribute)
            if (attribute->local_name() == local_name && attribute->namespace_uri() == namespace_uri)
                return attribute;
        return nullptr;
    }
} // namespace rapidxml
