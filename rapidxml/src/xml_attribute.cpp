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
} // namespace rapidxml
