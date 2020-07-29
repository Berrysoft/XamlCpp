#ifndef XAML_MARKUP_TEMPLATE_BASE_H
#define XAML_MARKUP_TEMPLATE_BASE_H

#include <xaml/meta/meta_macros.h>

XAML_CLASS(xaml_template_base, { 0xbbda09a5, 0xa66c, 0x466f, { 0x94, 0xc4, 0x8a, 0x3c, 0xe0, 0x51, 0x3e, 0xe6 } })

#define XAML_TEMPLATE_BASE_VTBL(type)         \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)) \
    XAML_METHOD(create, type, xaml_object*, xaml_object**)

XAML_DECL_INTERFACE_(xaml_template_base, xaml_object)
{
    XAML_DECL_VTBL(xaml_template_base, XAML_TEMPLATE_BASE_VTBL);
};

EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_template_base_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_template_base_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_MARKUP_TEMPLATE_BASE_H
