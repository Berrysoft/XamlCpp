#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <memory>
#include <rapidxml/memory_pool.hpp>
#include <rapidxml/xml_attribute.hpp>
#include <rapidxml/xml_node.hpp>

using namespace std;

namespace rapidxml
{
#ifdef WIN32
#define aligned_alloc(alignment, size) _aligned_malloc_dbg((size), (alignment), __FILE__, __LINE__)

#define aligned_free(ptr) _aligned_free_dbg(ptr)
#elif defined(__MINGW32__)
#define aligned_alloc(alignment, size) _aligned_malloc((size), (alignment))

#define aligned_free(ptr) _aligned_free(ptr)
#else

#define aligned_free(ptr) free(ptr)
#endif // WIN32 || __MINGW32__

    xml_node* memory_pool::allocate_node(node_type type, optional<string_view> name, optional<string_view> value)
    {
        void* memory = allocate_aligned(sizeof(xml_node));
        xml_node* node = new (memory) xml_node(type);
        if (name)
        {
            node->name(*name);
        }
        if (value)
        {
            node->value(*value);
        }
        return node;
    }

    xml_attribute* memory_pool::allocate_attribute(optional<string_view> name, optional<string_view> value)
    {
        void* memory = allocate_aligned(sizeof(xml_attribute));
        xml_attribute* attribute = new (memory) xml_attribute;
        if (name)
        {
            attribute->name(*name);
        }
        if (value)
        {
            attribute->value(*value);
        }
        return attribute;
    }

    char* memory_pool::allocate_string(optional<string_view> source, size_t size)
    {
        assert(source || size); // Either source or size (or both) must be specified
        if (size == 0)
            size = source->size() + 1;
        char* result = static_cast<char*>(allocate_aligned(size * sizeof(char)));
        source->copy(result, size);
        result[size - 1] = '\0';
        return result;
    }

    xml_node* memory_pool::clone_node(const xml_node* source, xml_node* result)
    {
        // Prepare result node
        if (result)
        {
            result->remove_all_attributes();
            result->remove_all_nodes();
            result->type(source->type());
        }
        else
            result = allocate_node(source->type());

        // Clone name and value
        result->qname(source->name(), source->local_offset());
        result->value(source->value());

        // Clone child nodes and attributes
        for (xml_node* child = source->first_node(); child; child = child->next_sibling())
            result->append_node(clone_node(child));
        for (xml_attribute* attr = source->first_attribute(); attr; attr = attr->next_attribute())
            result->append_attribute(allocate_attribute(attr->name(), attr->value()));

        return result;
    }

    void memory_pool::clear()
    {
        while (m_begin != m_static_memory)
        {
            void* previous_begin = reinterpret_cast<header*>(m_begin)->previous_begin;
            aligned_free(m_begin);
            m_begin = previous_begin;
        }
        init();
    }

    void memory_pool::init()
    {
        m_begin = m_static_memory;
        m_size = sizeof(m_static_memory);
        m_ptr = m_begin;
        void* result = align(alignment, 0, m_ptr, m_size);
        assert(result);
    }

    void* memory_pool::allocate_aligned(size_t size)
    {
        // Calculate aligned pointer
        void* result = align(alignment, size, m_ptr, m_size);

        // If not enough memory left in current pool, allocate a new pool
        if (!result)
        {
            // Calculate required pool size (may be bigger than RAPIDXML_DYNAMIC_POOL_SIZE)
            size_t pool_size = (max<size_t>)(dynamic_pool_size, size + sizeof(header));

            // Allocate
            void* pool = aligned_alloc(alignment, pool_size);

            // Setup new pool in allocated memory
            header* new_header = reinterpret_cast<header*>(pool);
            new_header->previous_begin = m_begin;
            m_begin = pool;
            m_ptr = (char*)pool + sizeof(header);
            m_size = pool_size - sizeof(header);

            // Calculate aligned pointer again using new pool
            result = align(alignment, 0, m_ptr, m_size);
            assert(result);
        }

        // Update pool and return aligned pointer
        m_ptr = (char*)result + size;
        return result;
    }
} // namespace rapidxml
