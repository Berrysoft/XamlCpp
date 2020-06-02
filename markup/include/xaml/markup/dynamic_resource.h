#ifndef XAML_MARKUP_DYNAMIC_RESOURCE_H
#define XAML_MARKUP_DYNAMIC_RESOURCE_H

#include <xaml/markup/markup_extension.h>
#include <xaml/meta/meta_macros.h>

XAML_CLASS(xaml_dynamic_resource, { 0xe2eb9034, 0xd5c8, 0x4e9d, { 0x9b, 0x8a, 0x31, 0x34, 0x20, 0x0c, 0x28, 0xd8 } })

#define XAML_DYNAMIC_RESOURCE_VTBL(type)                 \
    XAML_VTBL_INHERIT(XAML_MARKUP_EXTENSION_VTBL(type)); \
    XAML_PROP(key, type, xaml_string**, xaml_string*)

XAML_DECL_INTERFACE_(xaml_dynamic_resource, xaml_markup_extension)
{
    XAML_DECL_VTBL(xaml_dynamic_resource, XAML_DYNAMIC_RESOURCE_VTBL);
};

EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_dynamic_resource_new(xaml_dynamic_resource**) XAML_NOEXCEPT;
EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_dynamic_resource_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_dynamic_resource_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_MARKUP_DYNAMIC_RESOURCE_H
