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

    xml_node* memory_pool::allocate_node(node_type type)
    {
        void* memory = allocate_aligned(sizeof(xml_node));
        xml_node* node = new (memory) xml_node(type);
        return node;
    }

    xml_attribute* memory_pool::allocate_attribute()
    {
        void* memory = allocate_aligned(sizeof(xml_attribute));
        xml_attribute* attribute = new (memory) xml_attribute;
        return attribute;
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
            m_ptr = reinterpret_cast<char*>(pool) + sizeof(header);
            m_size = pool_size - sizeof(header);

            // Calculate aligned pointer again using new pool
            result = align(alignment, 0, m_ptr, m_size);
            assert(result);
        }

        // Update pool and return aligned pointer
        m_ptr = reinterpret_cast<char*>(result) + size;
        return result;
    }
} // namespace rapidxml
