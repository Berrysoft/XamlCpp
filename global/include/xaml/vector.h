#ifndef XAML_VECTOR_H
#define XAML_VECTOR_H

#ifdef __cplusplus
#include <vector>
#include <xaml/ptr.hpp>
#endif // __cplusplus

#include <xaml/enumerable.h>

XAML_CLASS(xaml_vector_view, { 0x8960a280, 0xddbb, 0x4b5b, { 0xb4, 0xeb, 0x27, 0x6d, 0xd3, 0x90, 0x6e, 0xd6 } })

#define XAML_VECTOR_VIEW_VTBL(type)                              \
    XAML_VTBL_INHERIT(XAML_ENUMERABLE_VTBL(type));               \
    XAML_METHOD(get_at, type, XAML_CSTD int32_t, xaml_object**); \
    XAML_METHOD(get_size, type, XAML_CSTD int32_t*)

XAML_DECL_INTERFACE_(xaml_vector_view, xaml_enumerable)
{
    XAML_DECL_VTBL(xaml_vector_view, XAML_VECTOR_VIEW_VTBL);
};

XAML_CLASS(xaml_vector, { 0xad5e7c14, 0x969d, 0x4e76, { 0x97, 0x6e, 0xc3, 0x17, 0xb4, 0x41, 0x12, 0x5e } })

#define XAML_VECTOR_VTBL(type)                                  \
    XAML_VTBL_INHERIT(XAML_VECTOR_VIEW_VTBL(type));             \
    XAML_METHOD(set_at, type, XAML_CSTD int32_t, xaml_object*); \
    XAML_METHOD(append, type, xaml_object*);                    \
    XAML_METHOD(remove_at, type, XAML_CSTD int32_t);            \
    XAML_METHOD(clear, type)

XAML_DECL_INTERFACE_(xaml_vector, xaml_vector_view)
{
    XAML_DECL_VTBL(xaml_vector, XAML_VECTOR_VTBL);
};

EXTERN_C XAML_API xaml_result XAML_CALL xaml_vector_new(xaml_vector**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_API xaml_result XAML_CALL xaml_vector_new(std::vector<xaml_ptr<xaml_object>>&&, xaml_vector**) noexcept;
#endif // __cplusplus

#endif // !XAML_VECTOR_H
