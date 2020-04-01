#include <cassert>
#include <internal.hpp>
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
        return node->type() == node_type::document ? static_cast<xml_document*>(node) : 0;
    }

    xml_node* xml_node::first_node(optional<string_view> name, bool case_sensitive) const
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

    xml_node* xml_node::first_node_ns(string_view namespace_uri, string_view local_name, bool local_name_case_sensitive) const
    {
        for (xml_node* child = m_first_node; child; child = child->next_sibling())
            if (internal::compare(child->local_name(),
                                  local_name, local_name_case_sensitive) &&
                internal::compare(child->namespace_uri(),
                                  namespace_uri))
                return child;
        return nullptr;
    }

    xml_node* xml_node::first_node_ns(string_view namespace_uri) const
    {
        for (xml_node* child = m_first_node; child; child = child->next_sibling())
            if (internal::compare(child->namespace_uri(), namespace_uri))
                return child;
        return nullptr;
    }

    xml_node* xml_node::last_node(optional<string_view> name, bool case_sensitive) const
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

    xml_node* xml_node::previous_sibling(optional<string_view> name, bool case_sensitive) const
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

    xml_node* xml_node::next_sibling(optional<string_view> name, bool case_sensitive) const
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

    xml_node* xml_node::next_sibling_ns(string_view namespace_uri, string_view local_name, bool local_name_case_sensitive) const
    {
        for (xml_node* sibling = m_next_sibling; sibling; sibling = sibling->m_next_sibling)
            if (internal::compare(sibling->local_name(),
                                  local_name, local_name_case_sensitive) &&
                internal::compare(sibling->namespace_uri(),
                                  namespace_uri))
                return sibling;
        return nullptr;
    }

    xml_node* xml_node::next_sibling_ns(string_view namespace_uri) const
    {
        for (xml_node* sibling = m_next_sibling; sibling; sibling = sibling->m_next_sibling)
            if (internal::compare(sibling->namespace_uri(), namespace_uri))
                return sibling;
        return nullptr;
    }

    xml_attribute* xml_node::first_attribute(optional<string_view> name, bool case_sensitive) const
    {
        if (name)
        {
            for (xml_attribute* attribute = m_first_attribute; attribute; attribute = attribute->m_next_attribute)
                if (internal::compare(attribute->name(), *name, case_sensitive))
                    return attribute;
            return nullptr;
        }
        else
            return m_first_attribute;
    }

    xml_attribute* xml_node::first_attribute_ns(string_view namespace_uri, string_view local_name, bool local_name_case_sensitive) const
    {
        for (xml_attribute* attribute = m_first_attribute; attribute; attribute = attribute->m_next_attribute)
            if (internal::compare(attribute->local_name(),
                                  local_name, local_name_case_sensitive) &&
                internal::compare(attribute->namespace_uri(),
                                  namespace_uri))
                return attribute;
        return nullptr;
    }

    xml_attribute* xml_node::last_attribute(optional<string_view> name, bool case_sensitive) const
    {
        if (name)
        {
            for (xml_attribute* attribute = m_last_attribute; attribute; attribute = attribute->m_prev_attribute)
                if (internal::compare(attribute->name(), *name, case_sensitive))
                    return attribute;
            return nullptr;
        }
        else
            return m_first_attribute ? m_last_attribute : 0;
    }

    void xml_node::prepend_node(xml_node* child)
    {
        assert(child && !child->parent() && child->type() != node_type::document);
        if (first_node())
        {
            child->m_next_sibling = m_first_node;
            m_first_node->m_prev_sibling = child;
        }
        else
        {
            child->m_next_sibling = 0;
            m_last_node = child;
        }
        m_first_node = child;
        child->m_parent = this;
        child->m_prev_sibling = 0;
    }

    void xml_node::append_node(xml_node* child)
    {
        assert(child && !child->parent() && child->type() != node_type::document);
        if (first_node())
        {
            child->m_prev_sibling = m_last_node;
            m_last_node->m_next_sibling = child;
        }
        else
        {
            child->m_prev_sibling = 0;
            m_first_node = child;
        }
        m_last_node = child;
        child->m_parent = this;
        child->m_next_sibling = 0;
    }

    void xml_node::insert_node(xml_node* where, xml_node* child)
    {
        assert(!where || where->parent() == this);
        assert(child && !child->parent() && child->type() != node_type::document);
        if (where == m_first_node)
            prepend_node(child);
        else if (where == 0)
            append_node(child);
        else
        {
            child->m_prev_sibling = where->m_prev_sibling;
            child->m_next_sibling = where;
            where->m_prev_sibling->m_next_sibling = child;
            where->m_prev_sibling = child;
            child->m_parent = this;
        }
    }

    void xml_node::remove_first_node()
    {
        assert(first_node());
        xml_node* child = m_first_node;
        m_first_node = child->m_next_sibling;
        if (child->m_next_sibling)
            child->m_next_sibling->m_prev_sibling = 0;
        else
            m_last_node = 0;
        child->m_parent = 0;
    }

    void xml_node::remove_last_node()
    {
        assert(first_node());
        xml_node* child = m_last_node;
        if (child->m_prev_sibling)
        {
            m_last_node = child->m_prev_sibling;
            child->m_prev_sibling->m_next_sibling = 0;
        }
        else
            m_first_node = 0;
        child->m_parent = 0;
    }

    void xml_node::remove_node(xml_node* where)
    {
        assert(where && where->parent() == this);
        assert(first_node());
        if (where == m_first_node)
            remove_first_node();
        else if (where == m_last_node)
            remove_last_node();
        else
        {
            where->m_prev_sibling->m_next_sibling = where->m_next_sibling;
            where->m_next_sibling->m_prev_sibling = where->m_prev_sibling;
            where->m_parent = 0;
        }
    }

    void xml_node::remove_all_nodes()
    {
        for (xml_node* node = first_node(); node; node = node->m_next_sibling)
            node->m_parent = 0;
        m_first_node = 0;
    }

    void xml_node::prepend_attribute(xml_attribute* attribute)
    {
        assert(attribute && !attribute->parent());
        if (first_attribute())
        {
            attribute->m_next_attribute = m_first_attribute;
            m_first_attribute->m_prev_attribute = attribute;
        }
        else
        {
            attribute->m_next_attribute = 0;
            m_last_attribute = attribute;
        }
        m_first_attribute = attribute;
        attribute->m_parent = this;
        attribute->m_prev_attribute = 0;
    }

    void xml_node::append_attribute(xml_attribute* attribute)
    {
        assert(attribute && !attribute->parent());
        if (first_attribute())
        {
            attribute->m_prev_attribute = m_last_attribute;
            m_last_attribute->m_next_attribute = attribute;
        }
        else
        {
            attribute->m_prev_attribute = 0;
            m_first_attribute = attribute;
        }
        m_last_attribute = attribute;
        attribute->m_parent = this;
        attribute->m_next_attribute = 0;
    }

    void xml_node::insert_attribute(xml_attribute* where, xml_attribute* attribute)
    {
        assert(!where || where->parent() == this);
        assert(attribute && !attribute->parent());
        if (where == m_first_attribute)
            prepend_attribute(attribute);
        else if (where == 0)
            append_attribute(attribute);
        else
        {
            attribute->m_prev_attribute = where->m_prev_attribute;
            attribute->m_next_attribute = where;
            where->m_prev_attribute->m_next_attribute = attribute;
            where->m_prev_attribute = attribute;
            attribute->m_parent = this;
        }
    }

    void xml_node::remove_first_attribute()
    {
        assert(first_attribute());
        xml_attribute* attribute = m_first_attribute;
        if (attribute->m_next_attribute)
        {
            attribute->m_next_attribute->m_prev_attribute = 0;
        }
        else
            m_last_attribute = 0;
        attribute->m_parent = 0;
        m_first_attribute = attribute->m_next_attribute;
    }

    void xml_node::remove_last_attribute()
    {
        assert(first_attribute());
        xml_attribute* attribute = m_last_attribute;
        if (attribute->m_prev_attribute)
        {
            attribute->m_prev_attribute->m_next_attribute = 0;
            m_last_attribute = attribute->m_prev_attribute;
        }
        else
            m_first_attribute = 0;
        attribute->m_parent = 0;
    }

    void xml_node::remove_attribute(xml_attribute* where)
    {
        assert(first_attribute() && where->parent() == this);
        if (where == m_first_attribute)
            remove_first_attribute();
        else if (where == m_last_attribute)
            remove_last_attribute();
        else
        {
            where->m_prev_attribute->m_next_attribute = where->m_next_attribute;
            where->m_next_attribute->m_prev_attribute = where->m_prev_attribute;
            where->m_parent = 0;
        }
    }

    void xml_node::remove_all_attributes()
    {
        for (xml_attribute* attribute = first_attribute(); attribute; attribute = attribute->m_next_attribute)
            attribute->m_parent = 0;
        m_first_attribute = 0;
    }
} // namespace rapidxml