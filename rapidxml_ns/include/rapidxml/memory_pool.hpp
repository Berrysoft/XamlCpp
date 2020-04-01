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
    //! Pool maintains <code>RAPIDXML_STATIC_POOL_SIZE</code> bytes of statically allocated memory.
    //! Until static memory is exhausted, no dynamic memory allocations are done.
    //! When static memory is exhausted, pool allocates additional blocks of memory of size <code>RAPIDXML_DYNAMIC_POOL_SIZE</code> each,
    //! by using global <code>new[]</code> and <code>delete[]</code> operators.
    //! This behaviour can be changed by setting custom allocation routines.
    //! Use set_allocator() function to set them.
    //! <br><br>
    //! Allocations for nodes, attributes and strings are aligned at <code>RAPIDXML_ALIGNMENT</code> bytes.
    //! This value defaults to the size of pointer on target architecture.
    //! <br><br>
    //! To obtain absolutely top performance from the parser,
    //! it is important that all nodes are allocated from a single, contiguous block of memory.
    //! Otherwise, cache misses when jumping between two (or more) disjoint blocks of memory can slow down parsing quite considerably.
    //! If required, you can tweak <code>RAPIDXML_STATIC_POOL_SIZE</code>, <code>RAPIDXML_DYNAMIC_POOL_SIZE</code> and <code>RAPIDXML_ALIGNMENT</code>
    //! to obtain best wasted memory to performance compromise.
    //! To do it, define their values before rapidxml_ns.hpp file is included.
    //! \param char chararacter type of created nodes.
    class memory_pool
    {
    public:
        // Size of static memory block of memory_pool.
        // Define RAPIDXML_STATIC_POOL_SIZE before including rapidxml_ns.hpp if you want to override the default value.
        // No dynamic memory allocations are performed by memory_pool until static memory is exhausted.
        static constexpr size_t static_pool_size = 64 * 1024;

        // Size of dynamic memory block of memory_pool.
        // Define RAPIDXML_DYNAMIC_POOL_SIZE before including rapidxml_ns.hpp if you want to override the default value.
        // After the static block is exhausted, dynamic blocks with approximately this size are allocated by memory_pool.
        static constexpr size_t dynamic_pool_size = 64 * 1024;

        // Memory allocation alignment.
        // Define RAPIDXML_ALIGNMENT before including rapidxml_ns.hpp if you want to override the default value, which is the size of pointer.
        // All memory allocations for nodes, attributes and strings will be aligned to this value.
        // This must be a power of 2 and at least 1, otherwise memory_pool will not work.
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
        //! \param name Name to assign to the node, or 0 to assign no name.
        //! \param value Value to assign to the node, or 0 to assign no value.
        //! \param name_size Size of name to assign, or 0 to automatically calculate size from name string.
        //! \param value_size Size of value to assign, or 0 to automatically calculate size from value string.
        //! \return Pointer to allocated node. This pointer will never be NULL.
        RAPIDXML_API xml_node* allocate_node(node_type type,
                                             std::optional<std::string_view> name = std::nullopt,
                                             std::optional<std::string_view> value = std::nullopt);

        //! Allocates a new attribute from the pool, and optionally assigns name and value to it.
        //! If the allocation request cannot be accomodated, this function will throw <code>std::bad_alloc</code>.
        //! \param name Name to assign to the attribute, or 0 to assign no name.
        //! \param value Value to assign to the attribute, or 0 to assign no value.
        //! \param name_size Size of name to assign, or 0 to automatically calculate size from name string.
        //! \param value_size Size of value to assign, or 0 to automatically calculate size from value string.
        //! \return Pointer to allocated attribute. This pointer will never be NULL.
        RAPIDXML_API xml_attribute* allocate_attribute(std::optional<std::string_view> name = std::nullopt,
                                                       std::optional<std::string_view> value = std::nullopt);

        //! Allocates a char array of given size from the pool, and optionally copies a given string to it.
        //! If the allocation request cannot be accomodated, this function will throw <code>std::bad_alloc</code>.
        //! \param source String to initialize the allocated memory with, or 0 to not initialize it.
        //! \param size Number of characters to allocate, or zero to calculate it automatically from source string length; if size is 0, source string must be specified and null terminated.
        //! \return Pointer to allocated char array. This pointer will never be NULL.
        RAPIDXML_API char* allocate_string(std::optional<std::string_view> source = std::nullopt, std::size_t size = 0);

        //! Clones an xml_node and its hierarchy of child nodes and attributes.
        //! Nodes and attributes are allocated from this memory pool.
        //! Names and values are not cloned, they are shared between the clone and the source.
        //! Result node can be optionally specified as a second parameter,
        //! in which case its contents will be replaced with cloned source node.
        //! This is useful when you want to clone entire document.
        //! \param source Node to clone.
        //! \param result Node to put results in, or 0 to automatically allocate result node
        //! \return Pointer to cloned node. This pointer will never be NULL.
        RAPIDXML_API xml_node* clone_node(const xml_node* source, xml_node* result = nullptr);

        //! Clears the pool.
        //! This causes memory occupied by nodes allocated by the pool to be freed.
        //! Any nodes or strings allocated from the pool will no longer be valid.
        RAPIDXML_API void clear();

    private:
        struct alignas(alignof(void*)) header
        {
            void* previous_begin;
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
