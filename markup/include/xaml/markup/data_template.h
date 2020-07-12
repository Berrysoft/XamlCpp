#ifndef XAML_MARKUP_DATA_TEMPLATE_H
#define XAML_MARKUP_DATA_TEMPLATE_H

#include <xaml/meta/meta_macros.h>

XAML_CLASS(xaml_data_template, { 0xa416dc5c, 0x8476, 0x4ec0, { 0xae, 0x92, 0x9d, 0x49, 0x90, 0xaa, 0xdc, 0x3f } })

#define XAML_DATA_TEMPLATE_VTBL(type)          \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(create, type, xaml_object*)

XAML_DECL_INTERFACE_(xaml_data_template, xaml_object)
{
    XAML_DECL_VTBL(xaml_data_template, XAML_DATA_TEMPLATE_VTBL);
};

EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_data_template_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_data_template_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_MARKUP_DATA_TEMPLATE_H
