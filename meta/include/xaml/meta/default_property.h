#ifndef XAML_META_DEFAULT_PROPERTY_H
#define XAML_META_DEFAULT_PROPERTY_H

#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_default_property, { 0xaf8c4c53, 0xa4a0, 0x4f46, { 0xbd, 0x28, 0x2a, 0xf6, 0xb2, 0xfe, 0xe1, 0x79 } })

#ifdef __cplusplus
struct xaml_default_property : xaml_object
{
    virtual xaml_result XAML_CALL get_default_property(xaml_string**) noexcept = 0;
};
#else

#endif // __cplusplus

EXTERN_C XAML_META_API xaml_result xaml_default_property_new(xaml_string*, xaml_default_property**) XAML_NOEXCEPT;

#endif // !XAML_META_DEFAULT_PROPERTY_H
