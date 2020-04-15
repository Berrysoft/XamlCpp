#ifndef XAML_STRING_H
#define XAML_STRING_H

#include <stdbool.h>
#include <xaml/object.h>

#ifdef UNICODE
typedef wchar_t xaml_char_t;
#else
typedef char xaml_char_t;
#endif // UNICODE

typedef struct xaml_string xaml_string;

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

EXTERN_C XAML_META_API xaml_result xaml_string_new(xaml_char_t const*, xaml_string**) XAML_NOEXCEPT;

#endif // !XAML_STRING_H
