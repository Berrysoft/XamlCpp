#ifndef RAPIDXML_ITERATORS_HPP_INCLUDED
#define RAPIDXML_ITERATORS_HPP_INCLUDED

#include <cassert>
#include <iterator>
#include <rapidxml/xml_attribute.hpp>
#include <rapidxml/xml_node.hpp>

namespace rapidxml
{
    //! Iterator of child nodes of xml_node
    class node_iterator
    {
    public:
        typedef xml_node value_type;
        typedef xml_node& reference;
        typedef xml_node* pointer;
        typedef std::ptrdiff_t difference_type;
        typedef std::bidirectional_iterator_tag iterator_category;

        constexpr node_iterator() noexcept : m_node(nullptr) {}

        node_iterator(xml_node* node) : m_node(node->first_node()) {}

        reference operator*() const
        {
            assert(m_node);
            return *m_node;
        }

        pointer operator->() const
        {
            assert(m_node);
            return m_node;
        }

        node_iterator& operator++()
        {
            assert(m_node);
            m_node = m_node->next_sibling();
            return *this;
        }

        node_iterator operator++(int)
        {
            node_iterator tmp = *this;
            operator++();
            return tmp;
        }

        node_iterator& operator--()
        {
            assert(m_node && m_node->previous_sibling());
            m_node = m_node->previous_sibling();
            return *this;
        }

        node_iterator operator--(int)
        {
            node_iterator tmp = *this;
            operator--();
            return tmp;
        }

        friend constexpr bool operator==(node_iterator const& lhs, node_iterator const& rhs) { return lhs.m_node == rhs.m_node; }
        friend constexpr bool operator!=(node_iterator const& lhs, node_iterator const& rhs) { return !(lhs == rhs); }

    private:
        xml_node* m_node;
    };

    constexpr node_iterator begin(node_iterator const& it) { return it; }
    constexpr node_iterator end(node_iterator const&) { return {}; }

    //! Iterator of child attributes of xml_node
    class attribute_iterator
    {
    public:
        typedef xml_attribute value_type;
        typedef xml_attribute& reference;
        typedef xml_attribute* pointer;
        typedef std::ptrdiff_t difference_type;
        typedef std::bidirectional_iterator_tag iterator_category;

        constexpr attribute_iterator() noexcept : m_attribute(nullptr) {}

        attribute_iterator(xml_node* node) : m_attribute(node->first_attribute()) {}

        reference operator*() const
        {
            assert(m_attribute);
            return *m_attribute;
        }

        pointer operator->() const
        {
            assert(m_attribute);
            return m_attribute;
        }

        attribute_iterator& operator++()
        {
            assert(m_attribute);
            m_attribute = m_attribute->next_attribute();
            return *this;
        }

        attribute_iterator operator++(int)
        {
            attribute_iterator tmp = *this;
            operator++();
            return tmp;
        }

        attribute_iterator& operator--()
        {
            assert(m_attribute && m_attribute->previous_attribute());
            m_attribute = m_attribute->previous_attribute();
            return *this;
        }

        attribute_iterator operator--(int)
        {
            attribute_iterator tmp = *this;
            operator--();
            return tmp;
        }

        friend constexpr bool operator==(attribute_iterator const& lhs, attribute_iterator const& rhs) { return lhs.m_attribute == rhs.m_attribute; }
        friend constexpr bool operator!=(attribute_iterator const& lhs, attribute_iterator const& rhs) { return !(lhs == rhs); }

    private:
        xml_attribute* m_attribute;
    };

    constexpr attribute_iterator begin(attribute_iterator const& it) { return it; }
    constexpr attribute_iterator end(attribute_iterator const&) { return {}; }
} // namespace rapidxml

#endif
