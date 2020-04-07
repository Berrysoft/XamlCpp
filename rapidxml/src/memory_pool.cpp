#include <version>

#ifndef __cpp_lib_memory_resource

#ifdef XAML_WIN32
#define _CRTDBG_MAP_ALLOC
#endif // XAML_WIN32

#include <algorithm>
#include <cstdlib>
#include <memory>
#include <rapidxml/memory_pool.hpp>
#include <rapidxml/xml_attribute.hpp>
#include <rapidxml/xml_node.hpp>

using namespace std;

namespace rapidxml
{
#ifdef XAML_WIN32
#define aligned_alloc(alignment, size) _aligned_malloc((size), (alignment))

#define aligned_free(ptr) _aligned_free(ptr)
#else

#define aligned_free(ptr) free(ptr)
#endif // XAML_WIN32

    memory_pool::memory_pool() : memory_resource()
    {
        init();
    }

    memory_pool::~memory_pool() { release(); }

    void memory_pool::release()
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
    }

    constexpr size_t offset(size_t size, size_t alignment)
    {
        if (size <= alignment)
            return alignment;
        else
        {
            size_t n = size / alignment;
            size_t mod = size % alignment;
            return mod ? n + 1 : n;
        }
    }

    void* memory_pool::do_allocate(size_t size, size_t alignment)
    {
        // Calculate aligned pointer
        void* result = align(alignment, size, m_ptr, m_size);

        // If not enough memory left in current pool, allocate a new pool
        if (!result)
        {
            // Calculate required pool size
            size_t pool_size = (max<size_t>)(dynamic_pool_size, size + offset(sizeof(header), alignment));

            // Allocate
            void* pool = aligned_alloc(alignment, pool_size);
            if (!pool) throw bad_alloc{};

            // Setup new pool in allocated memory
            header* new_header = reinterpret_cast<header*>(pool);
            new_header->previous_begin = m_begin;
            m_begin = pool;
            m_ptr = reinterpret_cast<char*>(pool) + sizeof(header);
            m_size = pool_size - sizeof(header);

            // Calculate aligned pointer again using new pool
            result = align(alignment, 0, m_ptr, m_size);
            if (!result) throw bad_alloc{};
        }

        // Update pool and return aligned pointer
        m_ptr = reinterpret_cast<char*>(result) + size;
        return result;
    }
} // namespace rapidxml

#endif // !__cpp_lib_memory_resource
