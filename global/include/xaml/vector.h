#ifndef XAML_VECTOR_H
#define XAML_VECTOR_H

#ifdef __cplusplus
#include <vector>
#include <xaml/xaml_ptr.hpp>
#endif // __cplusplus

#include <xaml/enumerable.h>

XAML_CLASS(xaml_vector_view, { 0x8960a280, 0xddbb, 0x4b5b, { 0xb4, 0xeb, 0x27, 0x6d, 0xd3, 0x90, 0x6e, 0xd6 } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_vector_view : xaml_enumerable
{
    virtual xaml_result XAML_CALL get_at(std::size_t, xaml_object**) noexcept = 0;
    virtual xaml_result XAML_CALL get_size(std::size_t*) noexcept = 0;
};
#else
#define XAML_VECTOR_VIEW_VTBL(type)                                     \
    xaml_result(XAML_CALL* get_at)(type* const, size_t, xaml_object**); \
    xaml_result(XAML_CALL* get_size)(type* const, size_t*);

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

XAML_CLASS(xaml_vector, { 0xad5e7c14, 0x969d, 0x4e76, { 0x97, 0x6e, 0xc3, 0x17, 0xb4, 0x41, 0x12, 0x5e } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_vector : xaml_vector_view
{
    virtual xaml_result XAML_CALL set_at(std::size_t, xaml_object*) noexcept = 0;
    virtual xaml_result XAML_CALL append(xaml_object*) noexcept = 0;
    virtual xaml_result XAML_CALL remove_at(std::size_t) noexcept = 0;
    virtual xaml_result XAML_CALL clear() noexcept = 0;
};
#else
#define XAML_VECTOR_VTBL(type)                                         \
    xaml_result(XAML_CALL* set_at)(type* const, size_t, xaml_object*); \
    xaml_result(XAML_CALL* append)(type* const, xaml_object*);         \
    xaml_result(XAML_CALL* remove_at)(type* const, size_t);            \
    xaml_result(XAML_CALL* clear)(type* const);

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

EXTERN_C XAML_API xaml_result xaml_vector_new(xaml_vector**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_API xaml_result xaml_vector_new(std::vector<xaml_ptr<xaml_object>>&&, xaml_vector**) noexcept;
#endif // __cplusplus

#endif // !XAML_VECTOR_H
