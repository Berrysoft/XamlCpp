#ifndef XAML_META_DEFAULT_PROPERTY_H
#define XAML_META_DEFAULT_PROPERTY_H

#include <xaml/meta/meta_context.h>
#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_default_property, { 0xaf8c4c53, 0xa4a0, 0x4f46, { 0xbd, 0x28, 0x2a, 0xf6, 0xb2, 0xfe, 0xe1, 0x79 } })

#define XAML_DEFAULT_PROPERTY_VTBL(type)       \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(get_default_property, type, xaml_string**)

XAML_DECL_INTERFACE_(xaml_default_property, xaml_object)
{
    XAML_DECL_VTBL(xaml_default_property, XAML_DEFAULT_PROPERTY_VTBL);
};

EXTERN_C XAML_META_API xaml_result XAML_CALL xaml_default_property_new(xaml_string*, xaml_default_property**) XAML_NOEXCEPT;
EXTERN_C XAML_META_API xaml_result XAML_CALL xaml_default_property_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_META_DEFAULT_PROPERTY_H
