#ifndef XAML_ENUMERABLE_HPP
#define XAML_ENUMERABLE_HPP

#include <xaml/object.h>

XAML_CLASS(xaml_enumerator, { 0x4f706e46, 0x5b78, 0x4504, { 0xbc, 0x4c, 0x4a, 0x0c, 0x7d, 0x34, 0x9e, 0x11 } })

#ifdef __cplusplus
struct xaml_enumerator : xaml_object
{
    virtual xaml_result XAML_CALL move_next() noexcept = 0;
    virtual xaml_result XAML_CALL get_current(xaml_object**) const noexcept = 0;
};
#else
#define XAML_ENUMERATOR_VTBL(type)                  \
    xaml_result(XAML_CALL* move_next)(type* const); \
    xaml_result(XAML_CALL* get_current)(type const* const, xaml_object**);

struct xaml_enumerator
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_enumerator)
        XAML_ENUMERATOR_VTBL(xaml_enumerator)
    } const* vtbl;
};
#endif // __cplusplus

XAML_CLASS(xaml_enumerable, { 0x7d0d584f, 0x9d47, 0x4375, { 0x8a, 0x4b, 0xab, 0x09, 0x0f, 0xc2, 0xb0, 0x95 } })

#ifdef __cplusplus
struct xaml_enumerable : xaml_object
{
    virtual xaml_result XAML_CALL get_enumerator(xaml_enumerator**) const noexcept = 0;
};
#else
#define XAML_ENUMERABLE_VTBL(type) \
    xaml_result(XAML_CALL* get_enumerator)(type const* const, xaml_enumerator**);

struct xaml_enumerable
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_enumerable)
        XAML_ENUMERABLE_VTBL(xaml_enumerable)
    } const* vtbl;
};
#endif // __cplusplus

#endif // !XAML_ENUMERABLE_HPP
