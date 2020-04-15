#ifndef XAML_VECTOR_H
#define XAML_VECTOR_H

#include <xaml/enumerable.h>

typedef struct xaml_vector_view xaml_vector_view;

#ifdef __cplusplus
struct xaml_vector_view : xaml_enumerable
{
    virtual xaml_result XAML_CALL get_at(size_t, xaml_object**) const noexcept = 0;
};
#else
#define XAML_VECTOR_VIEW_VTBL(type) \
    xaml_result(XAML_CALL* get_at)(type const* const, size_t, xaml_object**);

struct xaml_vector_view
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_vector_view)
        XAML_ENUMERABLE_VTBL(xaml_vector_view)
        XAML_VECTOR_VIEW_VTBL(xaml_vector_view)
    } const* vtbl;
};
#endif // __cplusplus

typedef struct xaml_vector xaml_vector;

#ifdef __cplusplus
struct xaml_vector : xaml_vector_view
{
    virtual xaml_result XAML_CALL set_at(size_t, xaml_object*) noexcept = 0;
    virtual xaml_result XAML_CALL append(xaml_object*) noexcept = 0;
    virtual xaml_result XAML_CALL remove_at(size_t) noexcept = 0;
};
#else
#define XAML_VECTOR_VTBL(type)                                         \
    xaml_result(XAML_CALL* set_at)(type* const, size_t, xaml_object*); \
    xaml_result(XAML_CALL* append)(type* const, xaml_object*);         \
    xaml_result(XAML_CALL* remove_at)(type* const, size_t);

struct xaml_vector
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_vector)
        XAML_ENUMERABLE_VTBL(xaml_vector)
        XAML_VECTOR_VIEW_VTBL(xaml_vector)
        XAML_VECTOR_VTBL(xaml_vector)
    } const* vtbl;
};
#endif // __cplusplus

#endif // !XAML_VECTOR_H
