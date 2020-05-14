#ifndef XAML_META_GUID_HPP
#define XAML_META_GUID_HPP

#ifdef __cplusplus
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#else
#include <assert.h>
#if defined(_MSC_VER) && defined(__clang__)
#define static_assert _Static_assert
#endif // _MSC_VER && __clang__
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#endif // __cplusplus

#include <xaml/utility.h>

typedef struct xaml_guid xaml_guid;

struct xaml_guid
{
    XAML_CSTD uint32_t data1;
    XAML_CSTD uint16_t data2;
    XAML_CSTD uint16_t data3;
    XAML_CSTD uint8_t data4[8];
};

#ifdef __cplusplus
constexpr bool operator==(xaml_guid const& lhs, xaml_guid const& rhs)
{
    return lhs.data1 == rhs.data1 && lhs.data2 == rhs.data2 && lhs.data3 == rhs.data3 && std::equal(std::begin(lhs.data4), std::end(lhs.data4), std::begin(rhs.data4), std::end(rhs.data4));
}
constexpr bool operator!=(xaml_guid const& lhs, xaml_guid const& rhs) { return !(lhs == rhs); }
#endif // __cplusplus

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

XAML_CONSTEXPR XAML_CSTD size_t hash_value(xaml_guid XAML_CONST_REF g) XAML_NOEXCEPT
{
    XAML_CSTD size_t* ptr = (XAML_CSTD size_t*)XAML_ADDRESSOF_REF(g);
#if SIZE_MAX == UINT64_MAX
    static_assert(sizeof(XAML_CSTD size_t) == sizeof(XAML_CSTD uint64_t), "Unknown 64-bit platform.");
    return ptr[0] ^ ptr[1];
#elif SIZE_MAX == UINT32_MAX
    static_assert(sizeof(XAML_CSTD size_t) == sizeof(XAML_CSTD uint32_t), "Unknown 32-bit platform.");
    return ptr[0] ^ ptr[1] ^ ptr[2] ^ ptr[3];
#else
#error Cannot determine platform architecture
    return ptr[0];
#endif
}

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
struct xaml_type_guid;

template <typename T>
inline constexpr xaml_guid xaml_type_guid_v = xaml_type_guid<T>::value;

template <typename T, typename = void>
struct xaml_type_has_guid : std::false_type
{
};

template <typename T>
struct xaml_type_has_guid<T, std::void_t<decltype(xaml_type_guid<T>{})>> : std::true_type
{
};

template <typename T>
inline constexpr bool xaml_type_has_guid_v = xaml_type_has_guid<T>::value;
#endif // __cplusplus

#ifndef XAML_TYPE_NAME
#define __XAML_TYPE_NAME_BASE(name, ...) \
    XAML_CONSTEXPR_VAR xaml_guid xaml_guid_##name = __VA_ARGS__;
#ifdef __cplusplus
#define XAML_TYPE_NAME(type, name, ...)                      \
    __XAML_TYPE_NAME_BASE(name, __VA_ARGS__)                 \
    template <>                                              \
    struct xaml_type_guid<type>                              \
    {                                                        \
        static constexpr xaml_guid value = xaml_guid_##name; \
    };
#else
#define XAML_TYPE_NAME(type, name, ...) __XAML_TYPE_NAME_BASE(name, __VA_ARGS__)
#endif // __cplusplus
#endif // !XAML_TYPE_NAME

#ifndef XAML_TYPE
#define XAML_TYPE(type, ...) XAML_TYPE_NAME(type, type, __VA_ARGS__)
#endif // !XAML_TYPE

#ifndef XAML_CLASS
#define XAML_CLASS(type, ...) \
    typedef struct type type; \
    XAML_TYPE(type, __VA_ARGS__)
#endif // !XAML_CLASS

XAML_TYPE(void, { 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } })

#endif // !XAML_META_GUID_HPP
