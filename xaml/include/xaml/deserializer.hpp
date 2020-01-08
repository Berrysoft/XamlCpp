#ifndef XAML_DESERIALIZE_HPP
#define XAML_DESERIALIZE_HPP

#include <map>
#include <xaml/xaml_node.hpp>

namespace xaml
{
    class deserializer
    {
    private:
        std::map<std::string, std::shared_ptr<meta_class>> symbols;

    public:
        deserializer() {}
        ~deserializer() {}

    private:
        XAML_API void deserialize_impl(std::shared_ptr<meta_class> mc, xaml_node& node, std::shared_ptr<meta_class> root);
        XAML_API void deserialize_extensions(xaml_node& node);

    public:
        XAML_API void deserialize(xaml_node& node, std::shared_ptr<meta_class> mc);
    };
} // namespace xaml

#endif // !XAML_DESERIALIZE_HPP
