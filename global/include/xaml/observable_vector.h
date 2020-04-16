#ifndef XAML_OBSERVABLE_VECTOR_H
#define XAML_OBSERVABLE_VECTOR_H

#include <xaml/callback.h>
#include <xaml/vector.h>

XAML_CLASS(xaml_observable_vector, { 0xc84cb35f, 0x0a1c, 0x40e2, { 0x8e, 0x1c, 0x2c, 0x43, 0x0b, 0x1b, 0xb6, 0xcf } })

#ifdef __cplusplus
struct xaml_observable_vector : xaml_vector
{
    virtual xaml_result XAML_CALL add_vector_changed(xaml_callback*, size_t*) noexcept = 0;
    virtual xaml_result XAML_CALL remove_vector_changed(size_t) noexcept = 0;
};
#else
#define XAML_OBSERVABLE_VECTOR_VTBL(type)                                             \
    xaml_result(XAML_CALL* add_vector_changed)(type* const, xaml_callback*, size_t*); \
    xaml_result(XAML_CALL* remove_vector_changed)(type* const, size_t);

struct xaml_observable_vector
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_observable_vector)
        XAML_ENUMERABLE_VTBL(xaml_observable_vector)
        XAML_VECTOR_VIEW_VTBL(xaml_observable_vector)
        XAML_VECTOR_VTBL(xaml_observable_vector)
        XAML_OBSERVABLE_VECTOR_VTBL(xaml_observable_vector)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_API xaml_result xaml_observable_vector_new(xaml_observable_vector** ptr) XAML_NOEXCEPT;

#endif // !XAML_OBSERVABLE_VECTOR_H
