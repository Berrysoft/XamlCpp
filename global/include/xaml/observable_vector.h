#ifndef XAML_OBSERVABLE_VECTOR_H
#define XAML_OBSERVABLE_VECTOR_H

#include <xaml/delegate.h>
#include <xaml/vector.h>

XAML_CLASS(xaml_observable_vector, { 0xc84cb35f, 0x0a1c, 0x40e2, { 0x8e, 0x1c, 0x2c, 0x43, 0x0b, 0x1b, 0xb6, 0xcf } })

#define XAML_OBSERVABLE_VECTOR_VTBL(type)                                     \
    XAML_VTBL_INHERIT(XAML_VECTOR_VTBL(type));                                \
    XAML_METHOD(add_vector_changed, type, xaml_delegate*, XAML_STD int32_t*); \
    XAML_METHOD(remove_vector_changed, type, XAML_STD int32_t)

XAML_DECL_INTERFACE_(xaml_observable_vector, xaml_vector)
{
    XAML_DECL_VTBL(xaml_observable_vector, XAML_OBSERVABLE_VECTOR_VTBL);
};

EXTERN_C XAML_API xaml_result XAML_CALL xaml_observable_vector_new(xaml_observable_vector** ptr) XAML_NOEXCEPT;

typedef enum xaml_vector_changed_action
{
    xaml_vector_changed_add,
    xaml_vector_changed_erase,
    xaml_vector_changed_replace,
    xaml_vector_changed_move,
    xaml_vector_changed_reset
} xaml_vector_changed_action;

XAML_CLASS(xaml_vector_changed_args, { 0xf081fd5b, 0xd6d3, 0x4262, { 0xa7, 0xc7, 0x5d, 0x25, 0x82, 0x2d, 0x3b, 0x0a } })

#define XAML_VECTOR_CHANGED_ARGS_VTBL(type)                     \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                  \
    XAML_METHOD(get_action, type, xaml_vector_changed_action*); \
    XAML_METHOD(get_new_items, type, xaml_vector_view**);       \
    XAML_METHOD(get_new_index, type, XAML_STD int32_t*);        \
    XAML_METHOD(get_old_items, type, xaml_vector_view**);       \
    XAML_METHOD(get_old_index, type, XAML_STD int32_t*)

XAML_DECL_INTERFACE_(xaml_vector_changed_args, xaml_object)
{
    XAML_DECL_VTBL(xaml_vector_changed_args, XAML_VECTOR_CHANGED_ARGS_VTBL);
};

EXTERN_C XAML_API xaml_result XAML_CALL xaml_vector_changed_args_new(xaml_vector_changed_action, xaml_vector_view* new_items, int32_t new_index, xaml_vector_view* old_items, int32_t old_index, xaml_vector_changed_args**) XAML_NOEXCEPT;

#endif // !XAML_OBSERVABLE_VECTOR_H
