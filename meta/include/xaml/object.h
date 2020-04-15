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
    virtual size_t XAML_CALL add_ref() = 0;
    virtual size_t XAML_CALL release() = 0;
    virtual xaml_result XAML_CALL query(xaml_guid XAML_CONST_REF, xaml_object**) = 0;

    template <typename T>
    xaml_result query(T** ptr)
    {
        return query(xaml_type_guid_v<T>, (xaml_object**)ptr);
    }
};
#else
struct xaml_object
{
    struct
    {
        size_t(XAML_CALL* add_ref)(xaml_object*);
        size_t(XAML_CALL* release)(xaml_object*);
        xaml_result(XAML_CALL* query)(xaml_object*, xaml_guid XAML_CONST_REF, xaml_object**);
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_META_API xaml_object* xaml_object_new(void);

#endif // !XAML_OBJECT_HPP
