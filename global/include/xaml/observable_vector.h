#ifndef XAML_OBSERVABLE_VECTOR_H
#define XAML_OBSERVABLE_VECTOR_H

#include <xaml/delegate.h>
#include <xaml/vector.h>

XAML_CLASS(xaml_observable_vector, { 0xc84cb35f, 0x0a1c, 0x40e2, { 0x8e, 0x1c, 0x2c, 0x43, 0x0b, 0x1b, 0xb6, 0xcf } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_observable_vector : xaml_vector
{
    virtual xaml_result XAML_CALL add_vector_changed(xaml_delegate*, size_t*) noexcept = 0;
    virtual xaml_result XAML_CALL remove_vector_changed(size_t) noexcept = 0;
};
#else
#define XAML_OBSERVABLE_VECTOR_VTBL(type)                                             \
    xaml_result(XAML_CALL* add_vector_changed)(type* const, xaml_delegate*, size_t*); \
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

typedef enum xaml_vector_changed_action
{
    xaml_vector_changed_add,
    xaml_vector_changed_erase,
    xaml_vector_changed_replace,
    xaml_vector_changed_move,
    xaml_vector_changed_reset
} xaml_vector_changed_action;

XAML_CLASS(xaml_vector_changed_args, { 0xf081fd5b, 0xd6d3, 0x4262, { 0xa7, 0xc7, 0x5d, 0x25, 0x82, 0x2d, 0x3b, 0x0a } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_vector_changed_args : xaml_object
{
    virtual xaml_result XAML_CALL get_action(xaml_vector_changed_action*) noexcept = 0;
    virtual xaml_result XAML_CALL get_new_items(xaml_vector_view**) noexcept = 0;
    virtual xaml_result XAML_CALL get_new_index(size_t*) noexcept = 0;
    virtual xaml_result XAML_CALL get_old_items(xaml_vector_view**) noexcept = 0;
    virtual xaml_result XAML_CALL get_old_index(size_t*) noexcept = 0;
};
#else
#define XAML_VECTOR_CHANGED_ARGS_VTBL(type)                                       \
    xaml_result(XAML_CALL* get_action)(type* const, xaml_vector_changed_action*); \
    xaml_result(XAML_CALL* get_new_items)(type* const, xaml_vector_view**);       \
    xaml_result(XAML_CALL* get_new_index)(type* const, size_t*);                  \
    xaml_result(XAML_CALL* get_old_items)(type* const, xaml_vector_view**);       \
    xaml_result(XAML_CALL* get_old_index)(type* const, size_t*);

struct xaml_vector_changed_args
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_vector_changed_args)
        XAML_VECTOR_CHANGED_ARGS_VTBL(xaml_vector_changed_args)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_API xaml_result xaml_vector_changed_args_new(xaml_vector_changed_action, xaml_vector_view* new_items, size_t new_index, xaml_vector_view* old_items, size_t old_index, xaml_vector_changed_args**) XAML_NOEXCEPT;

#endif // !XAML_OBSERVABLE_VECTOR_H
