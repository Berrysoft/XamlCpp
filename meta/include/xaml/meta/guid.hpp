#ifndef XAML_META_GUID_HPP
#define XAML_META_GUID_HPP

#include <cstddef>
#include <cstdint>
#include <ostream>

#if __has_include(<version>)
#include <version>
#endif // __has_include(<version>)

#if __has_include(<compare>)
#include <compare>
#endif // __has_include(<compare>)

namespace xaml
{
    // Unique type identifier
    struct guid
    {
        std::uint32_t data1;
        std::uint16_t data2;
        std::uint16_t data3;
        std::uint8_t data4[8];

#ifdef __cpp_impl_three_way_comparison
        auto operator<=>(guid const&) const = default;
#endif // __cpp_impl_three_way_comparison
    };

#ifndef __cpp_impl_three_way_comparison
    constexpr bool operator==(guid const& lhs, guid const& rhs)
    {
        return lhs.data1 == rhs.data1 && lhs.data2 == rhs.data2 && lhs.data3 == rhs.data3 && std::equal(std::begin(lhs.data4), std::end(lhs.data4), std::begin(rhs.data4), std::end(rhs.data4));
    }
    constexpr bool operator!=(guid const& lhs, guid const& rhs) { return !(lhs == rhs); }
#endif // !__cpp_impl_three_way_comparison

    template <typename T>
    struct type_guid
    {
    };

    template <typename T>
    inline constexpr guid type_guid_v = type_guid<T>::value;

    template <>
    struct type_guid<void>
    {
        static constexpr guid value{ 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
    };

    constexpr std::size_t hash_value(guid const& g) noexcept
    {
        std::size_t* ptr = (std::size_t*)&g;
#if SIZE_MAX == UINT64_MAX
        static_assert(sizeof(std::size_t) == sizeof(std::uint64_t));
        return ptr[0] ^ ptr[1];
#elif SIZE_MAX == UINT32_MAX
        static_assert(sizeof(std::size_t) == sizeof(std::uint32_t));
        return ptr[0] ^ ptr[1] ^ ptr[2] ^ ptr[3];
#else
#error Cannot determine platform architecture
        return ptr[0];
#endif
    }
} // namespace xaml

namespace std
{
    template <>
    struct hash<xaml::guid>
    {
        constexpr std::size_t operator()(xaml::guid const& g) const noexcept
        {
            return xaml::hash_value(g);
        }
    };
} // namespace std

#endif // !XAML_META_GUID_HPP
