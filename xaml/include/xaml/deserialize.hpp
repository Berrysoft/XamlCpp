#ifndef XAML_DESERIALIZE_HPP
#define XAML_DESERIALIZE_HPP

#include <xaml/parser.hpp>

namespace xaml
{
    class deserializer
    {
    private:
        parser reader;

    public:
        constexpr bool is_open() const noexcept { return reader.is_open(); }
        void open(std::string_view file);

        deserializer() : reader() {}
        deserializer(std::string_view file) : deserializer() { open(file); }
        ~deserializer() {}

    private:
        void deserialize_impl(std::shared_ptr<meta_class> mc, xaml_node& node, std::shared_ptr<meta_class> root);

    public:
        void deserialize(std::shared_ptr<meta_class> mc);
    };
} // namespace xaml

#endif // !XAML_DESERIALIZE_HPP
