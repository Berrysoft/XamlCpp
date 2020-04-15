#ifndef XAML_ENUMERABLE_HPP
#define XAML_ENUMERABLE_HPP

#include <xaml/object.h>

typedef struct xaml_enumerator xaml_enumerator;

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

typedef struct xaml_enumerable xaml_enumerable;

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
