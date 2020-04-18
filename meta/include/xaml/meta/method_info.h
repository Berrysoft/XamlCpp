#ifndef XAML_META_METHOD_INFO_H
#define XAML_META_METHOD_INFO_H

#include <xaml/delegate.h>

XAML_CLASS(xaml_method_info, { 0xac6df520, 0x582e, 0x46e3, { 0xbd, 0x8b, 0xfb, 0x30, 0x34, 0x3e, 0x16, 0xa1 } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_method_info : xaml_delegate
{
    virtual xaml_result XAML_CALL get_name(xaml_string**) noexcept = 0;
};
#else
#define XAML_METHOD_INFO_VTBL(type) \
    xaml_result(XAML_CALL* get_name)(type* const, xaml_string**);

struct xaml_method_info
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_method_info)
        XAML_DELEGATE_VTBL(xaml_method_info)
        XAML_METHOD_INFO_VTBL(xaml_method_info)
    } const* vtbl;
};
#endif // __cplusplus

#endif // !XAML_META_METHOD_INFO_H
