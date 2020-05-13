#ifndef XAML_MARKUP_MARKUP_EXTENSION_H
#define XAML_MARKUP_MARKUP_EXTENSION_H

#include <xaml/meta/meta_context.h>

XAML_CLASS(xaml_markup_context, { 0x32121ea5, 0xf85e, 0x4ccf, { 0x8c, 0xac, 0x53, 0x7f, 0xc1, 0x8f, 0x99, 0x8e } })

#define XAML_MARKUP_CONTEXT_VTBL(type)                      \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));              \
    XAML_METHOD(get_current_element, type, xaml_object**);  \
    XAML_METHOD(get_current_property, type, xaml_string**); \
    XAML_METHOD(find_element, type, xaml_string*, xaml_object**)

XAML_DECL_INTERFACE_(xaml_markup_context, xaml_object)
{
    XAML_DECL_VTBL(xaml_markup_context, XAML_MARKUP_CONTEXT_VTBL);
};

XAML_CLASS(xaml_markup_extension, { 0x22563874, 0x590b, 0x40d0, { 0x9b, 0xaa, 0x43, 0x59, 0x4c, 0x5c, 0xaa, 0x9b } })

#define XAML_MARKUP_EXTENSION_VTBL(type)       \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(provide, type, xaml_meta_context*, xaml_markup_context*)

XAML_DECL_INTERFACE_(xaml_markup_extension, xaml_object)
{
    XAML_DECL_VTBL(xaml_markup_extension, XAML_MARKUP_EXTENSION_VTBL);
};

#endif // !XAML_MARKUP_MARKUP_EXTENSION_H
