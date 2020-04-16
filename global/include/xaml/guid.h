#ifndef XAML_META_GUID_HPP
#define XAML_META_GUID_HPP

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <xaml/utility.h>

#ifdef __cplusplus
#include <functional>
#include <version>
#if __has_include(<compare>)
#include <compare>
#endif // __has_include(<compare>)
#endif // __cplusplus

typedef struct xaml_guid xaml_guid;

struct xaml_guid
{
    uint32_t data1;
    uint16_t data2;
    uint16_t data3;
    uint8_t data4[8];

#ifdef __cpp_impl_three_way_comparison
    auto operator<=>(xaml_guid const&) const = default;
#endif // __cpp_impl_three_way_comparison
};

#if defined(__cplusplus) && !defined(__cpp_impl_three_way_comparison)
constexpr bool operator==(xaml_guid const& lhs, xaml_guid const& rhs)
{
    return lhs.data1 == rhs.data1 && lhs.data2 == rhs.data2 && lhs.data3 == rhs.data3 && std::equal(std::begin(lhs.data4), std::end(lhs.data4), std::begin(rhs.data4), std::end(rhs.data4));
}
constexpr bool operator!=(xaml_guid const& lhs, xaml_guid const& rhs) { return !(lhs == rhs); }
#endif // __cplusplus && !__cpp_impl_three_way_comparison

#ifndef __cplusplus
inline bool xaml_guid_equal(xaml_guid const* lhs, xaml_guid const* rhs)
{
    return lhs->data1 == rhs->data1 &&
           lhs->data2 == rhs->data2 &&
           lhs->data3 == rhs->data3 &&
           lhs->data4[0] == rhs->data4[0] &&
           lhs->data4[1] == rhs->data4[1] &&
           lhs->data4[2] == rhs->data4[2] &&
           lhs->data4[3] == rhs->data4[3] &&
           lhs->data4[4] == rhs->data4[4] &&
           lhs->data4[5] == rhs->data4[5] &&
           lhs->data4[6] == rhs->data4[6] &&
           lhs->data4[7] == rhs->data4[7];
}
#endif // !__cplusplus

XAML_CONSTEXPR size_t hash_value(xaml_guid XAML_CONST_REF g) XAML_NOEXCEPT
{
    size_t* ptr = (size_t*)XAML_ADDRESSOF_REF(g);
#if SIZE_MAX == UINT64_MAX
    static_assert(sizeof(size_t) == sizeof(uint64_t), "Unknown 64-bit platform.");
    return ptr[0] ^ ptr[1];
#elif SIZE_MAX == UINT32_MAX
    static_assert(sizeof(size_t) == sizeof(uint32_t), "Unknown 32-bit platform.");
    return ptr[0] ^ ptr[1] ^ ptr[2] ^ ptr[3];
#else
#error Cannot determine platform architecture
    return ptr[0];
#endif
}

XAML_CONSTEXPR_VAR xaml_guid void_guid = { 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

#ifdef __cplusplus
namespace std
{
    template <>
    struct hash<xaml_guid>
    {
        constexpr std::size_t operator()(xaml_guid const& g) const noexcept
        {
            return hash_value(g);
        }
    };
} // namespace std

template <typename T>
struct xaml_type_guid
{
};

template <typename T>
inline constexpr xaml_guid xaml_type_guid_v = xaml_type_guid<T>::value;

template <>
struct xaml_type_guid<void>
{
    static constexpr xaml_guid value = void_guid;
};
#else
#define xaml_type_guid_v(type) (type##_guid)
#endif // __cplusplus

#ifndef XAML_CLASS
#define __XAML_CLASS_BASE(type, ...) \
    typedef struct type type;        \
    XAML_CONSTEXPR_VAR xaml_guid type##_guid = __VA_ARGS__;
#ifdef __cplusplus
#define XAML_CLASS(type, ...)                           \
    __XAML_CLASS_BASE(type, __VA_ARGS__)                \
    template <>                                         \
    struct xaml_type_guid<type>                         \
    {                                                   \
        static constexpr xaml_guid value = type##_guid; \
    };
#else
#define XAML_CLASS(type, ...) __XAML_CLASS_BASE(type, __VA_ARGS__)
#endif // __cplusplus
#endif // !XAML_CLASS

#endif // !XAML_META_GUID_HPP
