#ifndef XAML_STRING_H
#define XAML_STRING_H

#ifdef __cplusplus
#include <string>
#include <string_view>
#include <xaml/ptr.hpp>
#else
#include <stdbool.h>
#endif // __cplusplus

#include <xaml/object.h>

#ifdef UNICODE
typedef wchar_t xaml_char_t;
#else
typedef char xaml_char_t;
#endif // UNICODE

#ifndef U
#ifdef UNICODE
#define U(x) L##x
#else
#define U(x) x
#endif // UNICODE
#endif // !U

XAML_CLASS(xaml_string, { 0xc8386ec4, 0xd28d, 0x422f, { 0x9e, 0x44, 0x36, 0xaa, 0x77, 0x63, 0x39, 0xd3 } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_string : xaml_object
{
    virtual xaml_result XAML_CALL get_length(size_t*) noexcept = 0;
    virtual xaml_result XAML_CALL get_data(xaml_char_t const**) noexcept = 0;
    virtual xaml_result XAML_CALL equals(xaml_string*, bool*) noexcept = 0;
};
#else
#define XAML_STRING_VTBL(type)                                          \
    XAML_OBJECT_VTBL(type)                                              \
    xaml_result(XAML_CALL* get_length)(type* const, size_t*);           \
    xaml_result(XAML_CALL* get_data)(type* const, xaml_char_t const**); \
    xaml_result(XAML_CALL* equals)(type* const, xaml_string*, bool*);

struct xaml_string
{
    struct
    {
        XAML_STRING_VTBL(xaml_string)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_API xaml_result xaml_string_new(xaml_char_t const*, xaml_string**) XAML_NOEXCEPT;

#ifdef __cplusplus
typedef std::basic_string<xaml_char_t> xaml_std_string_t;
typedef std::basic_string_view<xaml_char_t> xaml_std_string_view_t;

XAML_API xaml_result xaml_string_new(xaml_std_string_t&&, xaml_string**) noexcept;
XAML_API xaml_result xaml_string_new(xaml_std_string_view_t, xaml_string**) noexcept;

inline xaml_std_string_view_t to_string_view_t(xaml_ptr<xaml_string> const& str)
{
    xaml_char_t const* data;
    XAML_THROW_IF_FAILED(str->get_data(&data));
    size_t length;
    XAML_THROW_IF_FAILED(str->get_length(&length));
    return xaml_std_string_view_t(data, length);
}

inline xaml_std_string_t to_string_t(xaml_ptr<xaml_string> const& str)
{
    return (xaml_std_string_t)to_string_view_t(str);
}
#endif // __cplusplus

#endif // !XAML_STRING_H
