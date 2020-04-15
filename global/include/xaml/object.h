#ifndef XAML_OBJECT_HPP
#define XAML_OBJECT_HPP

#include <stddef.h>
#include <xaml/guid.h>
#include <xaml/result.h>
#include <xaml/utility.h>

XAML_CLASS(xaml_object, { 0xaf86e2e0, 0xb12d, 0x4c6a, { 0x9c, 0x5a, 0xd7, 0xaa, 0x65, 0x10, 0x1e, 0x90 } })

#ifdef __cplusplus
struct xaml_object
{
    virtual size_t XAML_CALL add_ref() noexcept = 0;
    virtual size_t XAML_CALL release() noexcept = 0;
    virtual xaml_result XAML_CALL query(xaml_guid XAML_CONST_REF, xaml_object**) noexcept = 0;

    template <typename T>
    xaml_result query(T** ptr) noexcept
    {
        return query(xaml_type_guid_v<T>, (xaml_object**)ptr);
    }
};
#else
#define XAML_OBJECT_VTBL(type)               \
    size_t(XAML_CALL* add_ref)(type* const); \
    size_t(XAML_CALL* release)(type* const); \
    xaml_result(XAML_CALL* query)(type* const, xaml_guid XAML_CONST_REF, xaml_object**);

struct xaml_object
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_object)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_API xaml_result xaml_object_new(xaml_object**) XAML_NOEXCEPT;

#endif // !XAML_OBJECT_HPP
