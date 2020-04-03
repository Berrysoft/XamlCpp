#include <rapidxml/xml_attribute.hpp>
#include <rapidxml/xml_base.hpp>
#include <rapidxml/xml_node.hpp>

using namespace std;

namespace rapidxml
{
    xml_node* xml_base::document() const
    {
        if (xml_node* node = this->parent())
        {
            while (node->parent())
                node = node->parent();
            return node->type() == node_type::document ? node : nullptr;
        }
        else
            return nullptr;
    }
} // namespace rapidxml
