#ifndef RAPID_XML_MEMORY_POOL_HPP
#define RAPID_XML_MEMORY_POOL_HPP

#include <rapidxml/utility.hpp>
#include <rapidxml/xml_node.hpp>

namespace rapidxml
{
    //! This class is used by the parser to create new nodes and attributes, without overheads of dynamic memory allocation.
    //! In most cases, you will not need to use this class directly.
    //! However, if you need to create nodes manually or modify names/values of nodes,
    //! you are encouraged to use memory_pool of relevant xml_document to allocate the memory.
    //! Not only is this faster than allocating them by using <code>new</code> operator,
    //! but also their lifetime will be tied to the lifetime of document,
    //! possibly simplyfing memory management.
    //! <br><br>
    //! Call allocate_node() or allocate_attribute() functions to obtain new nodes or attributes from the pool.
    //! You can also call allocate_string() function to allocate strings.
    //! Such strings can then be used as names or values of nodes without worrying about their lifetime.
    //! Note that there is no <code>free()</code> function -- all allocations are freed at once when clear() function is called,
    //! or when the pool is destroyed.
    //! <br><br>
    //! It is also possible to create a standalone memory_pool, and use it
    //! to allocate nodes, whose lifetime will not be tied to any document.
    //! <br><br>
    class memory_pool
    {
    public:
        // Size of static memory block of memory_pool.
        // No dynamic memory allocations are performed by memory_pool until static memory is exhausted.
        static constexpr size_t static_pool_size = 64 * 1024;

        // Size of dynamic memory block of memory_pool.
        // After the static block is exhausted, dynamic blocks with approximately this size are allocated by memory_pool.
        static constexpr size_t dynamic_pool_size = 64 * 1024;

        // Memory allocation alignment.
        // All memory allocations for nodes, attributes and strings will be aligned to this value.
        static constexpr size_t alignment = alignof(void*);

    public:
        //! Constructs empty pool with default allocator functions.
        memory_pool()
        {
            init();
        }

        //! Destroys pool and frees all the memory.
        //! This causes memory occupied by nodes allocated by the pool to be freed.
        //! Nodes allocated from the pool are no longer valid.
        ~memory_pool()
        {
            clear();
        }

        //! Allocates a new node from the pool, and optionally assigns name and value to it.
        //! If the allocation request cannot be accomodated, this function will throw <code>std::bad_alloc</code>.
        //! \param type Type of node to create.
        //! \return Pointer to allocated node. This pointer will never be NULL.
        RAPIDXML_API xml_node* allocate_node(node_type type);

        //! Allocates a new attribute from the pool, and optionally assigns name and value to it.
        //! If the allocation request cannot be accomodated, this function will throw <code>std::bad_alloc</code>.
        //! \param name Name to assign to the attribute, or 0 to assign no name.
        //! \return Pointer to allocated attribute. This pointer will never be NULL.
        RAPIDXML_API xml_attribute* allocate_attribute();

        //! Clears the pool.
        //! This causes memory occupied by nodes allocated by the pool to be freed.
        //! Any nodes or strings allocated from the pool will no longer be valid.
        RAPIDXML_API void clear();

    private:
        struct header
        {
            alignas(alignment) void* previous_begin;
        };

        RAPIDXML_API void init();

        void* allocate_aligned(std::size_t size);

        void* m_begin{ nullptr }; // Start of raw memory making up current pool
        void* m_ptr{ nullptr }; // First free byte in current pool
        std::size_t m_size{ 0 };
        char m_static_memory[static_pool_size]{}; // Static raw memory
    };
} // namespace rapidxml

#endif // !RAPID_XML_MEMORY_POOL_HPP
