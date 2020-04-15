#ifndef XAML_STRING_H
#define XAML_STRING_H

#include <stdbool.h>
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
struct xaml_string : xaml_object
{
    virtual xaml_result XAML_CALL get_length(size_t*) const noexcept = 0;
    virtual xaml_result XAML_CALL get_data(xaml_char_t const**) const noexcept = 0;
    virtual xaml_result XAML_CALL equals(xaml_string*, bool*) const noexcept = 0;
};
#else
#define XAML_STRING_VTBL(type)                                                \
    xaml_result(XAML_CALL* get_length)(type const* const, size_t*);           \
    xaml_result(XAML_CALL* get_data)(type const* const, xaml_char_t const**); \
    xaml_result(XAML_CALL* equals)(type const* const, xaml_string*, bool*);

struct xaml_string
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_string)
        XAML_STRING_VTBL(xaml_string)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_API xaml_result xaml_string_new(xaml_char_t const*, xaml_string**) XAML_NOEXCEPT;

#endif // !XAML_STRING_H
