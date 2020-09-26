#ifndef XAML_META_ENUM_INFO_H
#define XAML_META_ENUM_INFO_H

#ifdef __cplusplus
    #include <xaml/ptr.hpp>
#endif // __cplusplus

#include <xaml/map.h>
#include <xaml/meta/reflection_info.h>

XAML_KEY_VALUE_PAIR_T_T_TYPE(XAML_T_O(xaml_string), XAML_T_V(int32_t))
XAML_ENUMERATOR_T_TYPE(XAML_T_O(xaml_key_value_pair__xaml_string____int32_t__))
XAML_MAP_VIEW_T_T_TYPE(XAML_T_O(xaml_string), XAML_T_V(int32_t))

XAML_CLASS(xaml_enum_info, { 0x51dcc841, 0xc0d0, 0x4c8b, { 0x9f, 0x9f, 0xed, 0x7b, 0x48, 0xa1, 0xa9, 0xd9 } })

#define XAML_ENUM_INFO_VTBL(type)                                                  \
    XAML_VTBL_INHERIT(XAML_REFLECTION_INFO_VTBL(type));                            \
    XAML_METHOD(get_values, type, XAML_MAP_VIEW_T_T_NAME(xaml_string, int32_t)**); \
    XAML_METHOD(get_value, type, xaml_string*, int32_t*)

XAML_DECL_INTERFACE_(xaml_enum_info, xaml_reflection_info)
{
    XAML_DECL_VTBL(xaml_enum_info, XAML_ENUM_INFO_VTBL);
};

EXTERN_C XAML_META_API xaml_result XAML_CALL xaml_enum_info_new(xaml_guid XAML_CONST_REF, xaml_string*, xaml_string*, XAML_MAP_VIEW_T_T_NAME(xaml_string, int32_t) *, xaml_enum_info**) XAML_NOEXCEPT;

#ifdef __cplusplus
template <typename T>
xaml_result XAML_CALL xaml_enum_info_new(xaml_string* name, xaml_string* file, xaml_map_view<xaml_string, std::int32_t>* map, xaml_enum_info** ptr) noexcept
{
    return xaml_enum_info_new(xaml_type_guid_v<T>, name, file, map, ptr);
}
#endif // __cplusplus

#endif // !XAML_META_ENUM_INFO_H
