#include <cassert>
#include <rapidxml/internal.hpp>
#include <rapidxml/xml_document.hpp>
#include <rapidxml/xml_node.hpp>

namespace rapidxml
{
    xml_document* xml_node::document() const
    {
        xml_node* node = const_cast<xml_node*>(this);
        while (node->parent())
            node = node->parent();
        return node->type() == node_type::document ? static_cast<xml_document*>(node) : 0;
    }

    xml_node* xml_node::first_node(std::optional<std::string_view> name, bool case_sensitive) const
    {
        if (name)
        {
            for (xml_node* child = m_first_node; child; child = child->next_sibling())
                if (internal::compare(child->name(), *name, case_sensitive))
                    return child;
            return nullptr;
        }
        else
            return m_first_node;
    }

    xml_node* xml_node::first_node_ns(std::string_view namespace_uri, std::string_view local_name, bool local_name_case_sensitive = true) const
    {
        for (xml_node* child = m_first_node; child; child = child->next_sibling())
            if (internal::compare(child->local_name(),
                                  local_name, local_name_case_sensitive) &&
                internal::compare(child->namespace_uri(),
                                  namespace_uri))
                return child;
        return 0;
    }

    xml_node* xml_node::first_node_ns(std::string_view namespace_uri) const
    {
        for (xml_node* child = m_first_node; child; child = child->next_sibling())
            if (internal::compare(child->namespace_uri(), namespace_uri))
                return child;
        return 0;
    }

    xml_node* xml_node::last_node(std::optional<std::string_view> name, bool case_sensitive) const
    {
        assert(m_first_node); // Cannot query for last child if node has no children
        if (name)
        {
            for (xml_node* child = m_last_node; child; child = child->previous_sibling())
                if (internal::compare(child->name(), *name, case_sensitive))
                    return child;
            return nullptr;
        }
        else
            return m_last_node;
    }

    xml_node* xml_node::previous_sibling(std::optional<std::string_view> name, bool case_sensitive) const
    {
        assert(this->m_parent); // Cannot query for siblings if node has no parent
        if (name)
        {
            for (xml_node* sibling = m_prev_sibling; sibling; sibling = sibling->m_prev_sibling)
                if (internal::compare(sibling->name(), *name, case_sensitive))
                    return sibling;
            return nullptr;
        }
        else
            return m_prev_sibling;
    }

    xml_node* xml_node::next_sibling(std::optional<std::string_view> name, bool case_sensitive) const
    {
        assert(this->m_parent); // Cannot query for siblings if node has no parent
        if (name)
        {
            for (xml_node* sibling = m_next_sibling; sibling; sibling = sibling->m_next_sibling)
                if (internal::compare(sibling->name(), *name, case_sensitive))
                    return sibling;
            return nullptr;
        }
        else
            return m_next_sibling;
    }
} // namespace rapidxml
