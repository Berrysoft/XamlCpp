#ifndef XAML_MARKUP_BINDING_H
#define XAML_MARKUP_BINDING_H

#include <xaml/converter.h>
#include <xaml/markup/markup_extension.h>
#include <xaml/meta/meta_macros.h>

EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_binding_mode_register(xaml_meta_context*) XAML_NOEXCEPT;

XAML_CLASS(xaml_binding, { 0x9b0c4424, 0x93f5, 0x45e7, { 0xb8, 0xcf, 0x60, 0x24, 0x27, 0x2d, 0x03, 0x4e } })

#define XAML_BINDING_VTBL(type)                                        \
    XAML_VTBL_INHERIT(XAML_MARKUP_EXTENSION_VTBL(type));               \
    XAML_PROP(element, type, xaml_string**, xaml_string*);             \
    XAML_PROP(path, type, xaml_string**, xaml_string*);                \
    XAML_PROP(mode, type, xaml_binding_mode*, xaml_binding_mode);      \
    XAML_PROP(converter, type, xaml_converter**, xaml_converter*);     \
    XAML_PROP(converter_parameter, type, xaml_object**, xaml_object*); \
    XAML_PROP(converter_language, type, xaml_string**, xaml_string*)

XAML_DECL_INTERFACE_(xaml_binding, xaml_markup_extension)
{
    XAML_DECL_VTBL(xaml_binding, XAML_BINDING_VTBL);
};

EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_binding_new(xaml_binding**) XAML_NOEXCEPT;
EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_binding_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_binding_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_MARKUP_BINDING_H
