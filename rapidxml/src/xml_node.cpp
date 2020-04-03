#include <cassert>
#include <rapidxml/xml_attribute.hpp>
#include <rapidxml/xml_document.hpp>
#include <rapidxml/xml_node.hpp>

using namespace std;

namespace rapidxml
{
    xml_document* xml_node::document() const
    {
        xml_node* node = const_cast<xml_node*>(this);
        while (node->parent())
            node = node->parent();
        return node->type() == node_type::document ? static_cast<xml_document*>(node) : nullptr;
    }
} // namespace rapidxml
