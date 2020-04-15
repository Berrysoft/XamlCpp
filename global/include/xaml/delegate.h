#ifndef XAML_DELEGATE_H
#define XAML_DELEGATE_H

#include <xaml/object.h>
#include <xaml/vector.h>

XAML_CLASS(xaml_delegate, { 0x3a1a793a, 0x8a83, 0x4d40, { 0x89, 0x62, 0x03, 0x9e, 0x00, 0x0c, 0x0f, 0xc3 } })

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

EXTERN_C XAML_API xaml_result xaml_delegate_new(xaml_result (*)(xaml_vector_view*, xaml_object**), xaml_delegate**) XAML_NOEXCEPT;

#endif // !XAML_DELEGATE_H
