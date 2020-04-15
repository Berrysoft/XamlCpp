#ifndef XAML_DELEGATE_H
#define XAML_DELEGATE_H

#include <xaml/object.h>
#include <xaml/vector.h>

typedef struct xaml_delegate xaml_delegate;

#ifdef __cplusplus
struct xaml_delegate : xaml_object
{
    virtual xaml_result XAML_CALL invoke(xaml_vector_view*, xaml_object**) const noexcept = 0;
};
#else
#define XAML_DELEGATE_VTBL(type) \
    xaml_result(XAML_CALL* invoke)(type const* const, xaml_vector_view*, xaml_object**);

struct xaml_delegate
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_delegate)
        XAML_DELEGATE_VTBL(xaml_delegate)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_META_API xaml_result xaml_delegate_new(xaml_result (*)(xaml_vector_view*, xaml_object**), xaml_delegate**) XAML_NOEXCEPT;

#endif // !XAML_DELEGATE_H
