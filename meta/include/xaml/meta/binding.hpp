#ifndef XAML_META_BINDING_HPP
#define XAML_META_BINDING_HPP

#include <xaml/meta/meta.hpp>
#include <xaml/utility.hpp>

namespace xaml
{
    enum class binding_mode
    {
        one_time = 0x0,
        one_way = 0x1,
        one_way_to_source = 0x2,
        two_way = one_way | one_way_to_source
    };

    constexpr int operator&(binding_mode lhs, binding_mode rhs) { return (int)lhs & (int)rhs; }
    constexpr int operator|(binding_mode lhs, binding_mode rhs) { return (int)lhs | (int)rhs; }

    inline std::string __get_property_changed_event_name(std::string_view name)
    {
        return (std::string)name + "_changed";
    }

    XAML_META_API std::size_t bind(meta_context& ctx, std::weak_ptr<meta_class> target, std::string_view target_prop, std::weak_ptr<meta_class> source, std::string_view source_prop, binding_mode mode = binding_mode::one_time);
    XAML_META_API void unbind(meta_context& ctx, std::size_t token);
} // namespace xaml

#endif // !XAML_META_BINDING_HPP
