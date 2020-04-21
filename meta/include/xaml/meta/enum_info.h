#ifndef XAML_META_ENUM_INFO_H
#define XAML_META_ENUM_INFO_H

#ifdef __cplusplus
#include <xaml/ptr.hpp>
#endif // __cplusplus

#include <xaml/box.h>
#include <xaml/map.h>
#include <xaml/meta/reflection_info.h>

XAML_CLASS(xaml_enum_info, { 0x51dcc841, 0xc0d0, 0x4c8b, { 0x9f, 0x9f, 0xed, 0x7b, 0x48, 0xa1, 0xa9, 0xd9 } })

#define XAML_ENUM_INFO_VTBL(type)                       \
    XAML_VTBL_INHERIT(XAML_REFLECTION_INFO_VTBL(type)); \
    XAML_METHOD(get_values, type, xaml_map_view**);     \
    XAML_METHOD(get_value, type, xaml_string*, xaml_box**)

XAML_DECL_INTERFACE_(xaml_enum_info, xaml_reflection_info)
{
    XAML_DECL_VTBL(xaml_enum_info, XAML_ENUM_INFO_VTBL);
};

EXTERN_C XAML_META_API xaml_result xaml_enum_info_new(xaml_guid XAML_CONST_REF, xaml_string*, xaml_string*, xaml_map_view*, xaml_enum_info**) XAML_NOEXCEPT;

#endif // !XAML_META_ENUM_INFO_H
