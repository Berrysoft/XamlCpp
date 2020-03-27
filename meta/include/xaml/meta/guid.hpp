#ifndef XAML_META_GUID_HPP
#define XAML_META_GUID_HPP

#include <cstddef>
#include <cstdint>

namespace xaml
{
    // Unique type identifier
    struct guid
    {
        std::uint32_t data1;
        std::uint16_t data2;
        std::uint16_t data3;
        std::uint8_t data4[8];
    };

    constexpr bool operator==(guid const& lhs, guid const& rhs) { return lhs.data1 == rhs.data1 && lhs.data2 == rhs.data2 && lhs.data3 == rhs.data3 && std::equal(std::begin(lhs.data4), std::end(lhs.data4), std::begin(rhs.data4), std::end(rhs.data4)); }
    constexpr bool operator!=(guid const& lhs, guid const& rhs) { return !(lhs == rhs); }

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
} // namespace xaml

namespace std
{
    template <>
    struct hash<xaml::guid>
    {
        constexpr std::size_t operator()(xaml::guid const& g) const noexcept
        {
            if constexpr (sizeof(std::size_t) == sizeof(std::uint32_t))
            {
                std::size_t* ptr = (std::size_t*)&g;
                return ptr[0] ^ ptr[1] ^ ptr[2] ^ ptr[3];
            }
            else
            {
                static_assert(sizeof(std::size_t) == sizeof(std::uint64_t));
                std::size_t* ptr = (std::size_t*)&g;
                return ptr[0] ^ ptr[1];
            }
        }
    };
} // namespace std

#endif // !XAML_META_GUID_HPP
