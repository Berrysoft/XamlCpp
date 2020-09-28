#ifndef XAML_MARKUP_ELEMENT_BASE_H
#define XAML_MARKUP_ELEMENT_BASE_H

#include <xaml/map.h>
#include <xaml/meta/meta_context.h>
#include <xaml/meta/meta_macros.h>
#include <xaml/weak_reference.h>

#ifndef xaml_key_value_pair_2__xaml_string__xaml_object_defined
    #define xaml_key_value_pair_2__xaml_string__xaml_object_defined
XAML_KEY_VALUE_PAIR_2_TYPE(XAML_T_O(xaml_string), XAML_T_O(xaml_object))
#endif // !xaml_key_value_pair_2__xaml_string__xaml_object_defined

#ifndef xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_object_defined
    #define xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_object_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_key_value_pair_2__xaml_string__xaml_object))
#endif // !xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_object_defined

#ifndef xaml_map_view_2__xaml_string__xaml_object_defined
    #define xaml_map_view_2__xaml_string__xaml_object_defined
XAML_MAP_VIEW_2_TYPE(XAML_T_O(xaml_string), XAML_T_O(xaml_object))
#endif // !xaml_map_view_2__xaml_string__xaml_object_defined

XAML_CLASS(xaml_element_base, { 0x326263f6, 0x2e97, 0x4b7d, { 0xb2, 0xbd, 0x29, 0x4c, 0x49, 0x6a, 0x5a, 0xfe } })

#define XAML_ELEMENT_BASE_VTBL(type)                                                    \
    XAML_VTBL_INHERIT(XAML_WEAK_REFERENCE_SOURCE_VTBL(type));                           \
    XAML_METHOD(add_resource, type, xaml_string*, xaml_object*);                        \
    XAML_METHOD(get_resource, type, xaml_string*, xaml_object**);                       \
    XAML_METHOD(get_resources, type, XAML_MAP_VIEW_2_NAME(xaml_string, xaml_object)**); \
    XAML_PROP(parent, type, xaml_element_base**, xaml_element_base*);                   \
    XAML_EVENT(parent_changed, type, xaml_object, xaml_element_base)

XAML_DECL_INTERFACE_(xaml_element_base, xaml_weak_reference_source)
{
    XAML_DECL_VTBL(xaml_element_base, XAML_ELEMENT_BASE_VTBL);
};

EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_element_base_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_element_base_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_MARKUP_ELEMENT_BASE_H
