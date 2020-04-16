#ifndef XAML_CALLBACK_H
#define XAML_CALLBACK_H

#include <xaml/object.h>

XAML_CLASS(xaml_callback, { 0x76347aae, 0xb599, 0x4c2e, { 0xa2, 0x02, 0x18, 0x33, 0xca, 0x41, 0xfb, 0x26 } })

#ifdef __cplusplus
struct xaml_callback : xaml_object
{
    virtual xaml_result XAML_CALL invoke(xaml_object*, xaml_object*) const noexcept = 0;
};
#else
#define XAML_CALLBACK_VTBL(type) \
    xaml_result(XAML_CALL* invoke)(type const* const, xaml_object*, xaml_object*);

struct xaml_callback
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_callback)
        XAML_CALLBACK_VTBL(xaml_callback)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_API xaml_result xaml_callback_new(xaml_result (*)(xaml_object*, xaml_object*), xaml_callback**) XAML_NOEXCEPT;

#endif // !XAML_CALLBACK_H
