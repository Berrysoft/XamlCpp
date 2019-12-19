#ifndef XAML_DESERIALIZE_HPP
#define XAML_DESERIALIZE_HPP

#include <memory>
#include <xaml/meta/meta.hpp>

namespace xaml
{
    class xaml_deserializer
    {
    public:
        std::shared_ptr<meta_class> deserialize();
        void deserialize(std::shared_ptr<meta_class> mc);
    };
} // namespace xaml

#endif // !XAML_DESERIALIZE_HPP
