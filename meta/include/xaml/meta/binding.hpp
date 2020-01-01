#ifndef XAML_META_BINDING_HPP
#define XAML_META_BINDING_HPP

#include <xaml/meta/event.hpp>
#include <xaml/meta/meta.hpp>
#include <xaml/meta/conv.hpp>

namespace xaml
{
    enum class binding_mode
    {
        one_time = 0x0,
        one_way = 0x1,
        one_way_to_source = 0x2,
        two_way = one_way | one_way_to_source
    };

    constexpr bool operator!(binding_mode e) { return !(int)e; }
    constexpr binding_mode operator&(binding_mode lhs, binding_mode rhs) { return binding_mode{ (int)lhs & (int)rhs }; }
    constexpr binding_mode operator|(binding_mode lhs, binding_mode rhs) { return binding_mode{ (int)lhs | (int)rhs }; }

    class __binding_guard
    {
    private:
        using token_type = typename event<>::token_type;

        std::shared_ptr<meta_class> target;
        std::shared_ptr<meta_class> source;

        property_info target_prop;
        event_info target_event;
        property_info source_prop;
        event_info source_event;

        token_type target_token;
        token_type source_token;

    public:
        __binding_guard(std::shared_ptr<meta_class> target, std::string_view target_prop, std::shared_ptr<meta_class> source, std::string_view source_prop, binding_mode mode = binding_mode::one_time);
        ~__binding_guard();
    };

    std::size_t bind(std::shared_ptr<meta_class> target, std::string_view target_prop, std::shared_ptr<meta_class> source, std::string_view source_prop, binding_mode mode = binding_mode::one_time);
    void unbind(std::size_t token);
} // namespace xaml

#endif // !XAML_META_BINDING_HPP
