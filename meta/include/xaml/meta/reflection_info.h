#ifndef XAML_META_REFLECTION_INFO_H
#define XAML_META_REFLECTION_INFO_H

#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_reflection_info, { 0x5abbe92c, 0x833d, 0x4d24, { 0xa7, 0x06, 0x1d, 0x5c, 0xe5, 0x14, 0x1b, 0x0a } })

#define XAML_REFLECTION_INFO_VTBL(type)         \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));  \
    XAML_METHOD(get_type, type, xaml_guid*);    \
    XAML_METHOD(get_name, type, xaml_string**); \
    XAML_METHOD(get_include_file, type, xaml_string**)

XAML_DECL_INTERFACE_(xaml_reflection_info, xaml_object)
{
    XAML_DECL_VTBL(xaml_reflection_info, XAML_REFLECTION_INFO_VTBL);
};

#endif // !XAML_META_REFLECTION_INFO_H
