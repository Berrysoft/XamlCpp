#ifndef XAML_OBJECT_HPP
#define XAML_OBJECT_HPP

#include <stddef.h>
#include <xaml/guid.h>
#include <xaml/result.h>
#include <xaml/utility.h>

typedef struct xaml_object xaml_object;

#ifdef __cplusplus
struct xaml_object
{
    virtual size_t XAML_CALL add_ref() noexcept = 0;
    virtual size_t XAML_CALL release() noexcept = 0;
    virtual xaml_result XAML_CALL query(xaml_guid XAML_CONST_REF, xaml_object**) const noexcept = 0;

    template <typename T>
    xaml_result query(T** ptr) const noexcept
    {
        return query(xaml_type_guid_v<T>, (xaml_object**)ptr);
    }
};
#else
#define XAML_OBJECT_VTBL(type)                      \
    size_t(XAML_CALL* add_ref)(type* const); \
    size_t(XAML_CALL* release)(type* const); \
    xaml_result(XAML_CALL* query)(type const* const, xaml_guid XAML_CONST_REF, xaml_object**);

struct xaml_object
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_object)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_META_API xaml_object* xaml_object_new(void) XAML_NOEXCEPT;

#endif // !XAML_OBJECT_HPP
