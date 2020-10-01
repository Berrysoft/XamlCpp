#ifndef XAML_META_TYPE_INFO_H
#define XAML_META_TYPE_INFO_H

#include <xaml/delegate.h>
#include <xaml/map.h>
#include <xaml/meta/collection_property_info.h>
#include <xaml/meta/event_info.h>
#include <xaml/meta/method_info.h>
#include <xaml/meta/property_info.h>
#include <xaml/meta/reflection_info.h>

XAML_CLASS(xaml_basic_type_info, { 0x373d2efe, 0x469b, 0x4cb9, { 0x97, 0xbd, 0xff, 0x33, 0xee, 0xfd, 0xbc, 0x3a } })

#define XAML_BASIC_TYPE_INFO_VTBL(type) XAML_VTBL_INHERIT(XAML_REFLECTION_INFO_VTBL(type))

XAML_DECL_INTERFACE_(xaml_basic_type_info, xaml_reflection_info)
{
    XAML_DECL_VTBL(xaml_basic_type_info, XAML_BASIC_TYPE_INFO_VTBL);
};

EXTERN_C XAML_META_API xaml_result XAML_CALL xaml_basic_type_info_new(xaml_guid XAML_CONST_REF, xaml_string*, xaml_string*, xaml_basic_type_info**) XAML_NOEXCEPT;

#ifndef xaml_hasher_1__xaml_guid_defined
    #define xaml_hasher_1__xaml_guid_defined
XAML_HASHER_1_TYPE(XAML_T_V(xaml_guid))
#endif // !xaml_hasher_1__xaml_guid_defined

#ifndef xaml_key_value_pair_2__xaml_guid__xaml_object_defined
    #define xaml_key_value_pair_2__xaml_guid__xaml_object_defined
XAML_KEY_VALUE_PAIR_2_TYPE(XAML_T_V(xaml_guid), XAML_T_O(xaml_object))
#endif // !xaml_key_value_pair_2__xaml_guid__xaml_object_defined

#ifndef xaml_enumerator_1__xaml_key_value_pair_2__xaml_guid__xaml_object_defined
    #define xaml_enumerator_1__xaml_key_value_pair_2__xaml_guid__xaml_object_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_key_value_pair_2__xaml_guid__xaml_object))
#endif // !xaml_enumerator_1__xaml_key_value_pair_2__xaml_guid__xaml_object_defined

#ifndef xaml_map_view_2__xaml_guid__xaml_object_defined
    #define xaml_map_view_2__xaml_guid__xaml_object_defined
XAML_MAP_VIEW_2_TYPE(XAML_T_V(xaml_guid), XAML_T_O(xaml_object))
#endif // !xaml_map_view_2__xaml_guid__xaml_object_defined

#ifndef xaml_key_value_pair_2__xaml_string__xaml_method_info_defined
    #define xaml_key_value_pair_2__xaml_string__xaml_method_info_defined
XAML_KEY_VALUE_PAIR_2_TYPE(XAML_T_O(xaml_string), XAML_T_O(xaml_method_info))
#endif // !xaml_key_value_pair_2__xaml_string__xaml_method_info_defined

#ifndef xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_method_info_defined
    #define xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_method_info_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_key_value_pair_2__xaml_string__xaml_method_info))
#endif // !xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_method_info_defined

#ifndef xaml_map_view_2__xaml_string__xaml_method_info_defined
    #define xaml_map_view_2__xaml_string__xaml_method_info_defined
XAML_MAP_VIEW_2_TYPE(XAML_T_O(xaml_string), XAML_T_O(xaml_method_info))
#endif // !xaml_map_view_2__xaml_string__xaml_method_info_defined

#ifndef xaml_key_value_pair_2__xaml_string__xaml_property_info_defined
    #define xaml_key_value_pair_2__xaml_string__xaml_property_info_defined
XAML_KEY_VALUE_PAIR_2_TYPE(XAML_T_O(xaml_string), XAML_T_O(xaml_property_info))
#endif // !xaml_key_value_pair_2__xaml_string__xaml_property_info_defined

#ifndef xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_property_info_defined
    #define xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_property_info_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_key_value_pair_2__xaml_string__xaml_property_info))
#endif // !xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_property_info_defined

#ifndef xaml_map_view_2__xaml_string__xaml_property_info_defined
    #define xaml_map_view_2__xaml_string__xaml_property_info_defined
XAML_MAP_VIEW_2_TYPE(XAML_T_O(xaml_string), XAML_T_O(xaml_property_info))
#endif // !xaml_map_view_2__xaml_string__xaml_property_info_defined

#ifndef xaml_key_value_pair_2__xaml_string__xaml_collection_property_info_defined
    #define xaml_key_value_pair_2__xaml_string__xaml_collection_property_info_defined
XAML_KEY_VALUE_PAIR_2_TYPE(XAML_T_O(xaml_string), XAML_T_O(xaml_collection_property_info))
#endif // !xaml_key_value_pair_2__xaml_string__xaml_collection_property_info_defined

#ifndef xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_collection_property_info_defined
    #define xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_collection_property_info_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_key_value_pair_2__xaml_string__xaml_collection_property_info))
#endif // !xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_collection_property_info_defined

#ifndef xaml_map_view_2__xaml_string__xaml_collection_property_info_defined
    #define xaml_map_view_2__xaml_string__xaml_collection_property_info_defined
XAML_MAP_VIEW_2_TYPE(XAML_T_O(xaml_string), XAML_T_O(xaml_collection_property_info))
#endif // !xaml_map_view_2__xaml_string__xaml_collection_property_info_defined

#ifndef xaml_key_value_pair_2__xaml_string__xaml_event_info_defined
    #define xaml_key_value_pair_2__xaml_string__xaml_event_info_defined
XAML_KEY_VALUE_PAIR_2_TYPE(XAML_T_O(xaml_string), XAML_T_O(xaml_event_info))
#endif // !xaml_key_value_pair_2__xaml_string__xaml_event_info_defined

#ifndef xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_event_info_defined
    #define xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_event_info_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_key_value_pair_2__xaml_string__xaml_event_info))
#endif // !xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_event_info_defined

#ifndef xaml_map_view_2__xaml_string__xaml_event_info_defined
    #define xaml_map_view_2__xaml_string__xaml_event_info_defined
XAML_MAP_VIEW_2_TYPE(XAML_T_O(xaml_string), XAML_T_O(xaml_event_info))
#endif // !xaml_map_view_2__xaml_string__xaml_event_info_defined

XAML_CLASS(xaml_type_info, { 0x3de3b2c1, 0x09d6, 0x433c, { 0xbf, 0x40, 0x40, 0x2d, 0xfe, 0x28, 0xda, 0x1d } })

#define XAML_TYPE_INFO_VTBL(type)                                                                                     \
    XAML_VTBL_INHERIT(XAML_REFLECTION_INFO_VTBL(type));                                                               \
    XAML_METHOD(get_attributes, type, XAML_MAP_VIEW_2_NAME(xaml_guid, xaml_object)**);                                \
    XAML_METHOD(get_attribute, type, xaml_guid XAML_CONST_REF, void**);                                               \
    XAML_METHOD(get_constructor, type, xaml_constructor_info**);                                                      \
    XAML_METHOD(get_methods, type, XAML_MAP_VIEW_2_NAME(xaml_string, xaml_method_info)**);                            \
    XAML_METHOD(get_method, type, xaml_string*, xaml_method_info**);                                                  \
    XAML_METHOD(get_properties, type, XAML_MAP_VIEW_2_NAME(xaml_string, xaml_property_info)**);                       \
    XAML_METHOD(get_property, type, xaml_string*, xaml_property_info**);                                              \
    XAML_METHOD(get_collection_properties, type, XAML_MAP_VIEW_2_NAME(xaml_string, xaml_collection_property_info)**); \
    XAML_METHOD(get_collection_property, type, xaml_string*, xaml_collection_property_info**);                        \
    XAML_METHOD(get_events, type, XAML_MAP_VIEW_2_NAME(xaml_string, xaml_event_info)**);                              \
    XAML_METHOD(get_event, type, xaml_string*, xaml_event_info**)

XAML_DECL_INTERFACE_(xaml_type_info, xaml_reflection_info)
{
    XAML_DECL_VTBL(xaml_type_info, XAML_TYPE_INFO_VTBL);

#ifdef __cplusplus
    template <typename T>
    xaml_result XAML_CALL get_attribute(T * *ptr) noexcept
    {
        return get_attribute(xaml_type_guid_v<T>, (void**)ptr);
    }

    xaml_result XAML_CALL construct(xaml_object * *ptr) noexcept
    {
        xaml_ptr<xaml_constructor_info> ctor;
        XAML_RETURN_IF_FAILED(get_constructor(&ctor));
        if (!ctor) return XAML_E_NOTIMPL;
        return ctor->invoke(ptr);
    }
#endif // __cplusplus
};

XAML_CLASS(xaml_type_info_registration, { 0x18aecfb7, 0x7fd3, 0x44a2, { 0xba, 0xc7, 0x1b, 0x2d, 0x75, 0xb1, 0x4f, 0xc9 } })

#define XAML_TYPE_INFO_REGISTRATION_VTBL(type)                                  \
    XAML_VTBL_INHERIT(XAML_TYPE_INFO_VTBL(type));                               \
    XAML_METHOD(add_attribute, type, xaml_guid XAML_CONST_REF, xaml_object*);   \
    XAML_METHOD(set_constructor, type, xaml_constructor_info*);                 \
    XAML_METHOD(add_method, type, xaml_method_info*);                           \
    XAML_METHOD(add_property, type, xaml_property_info*);                       \
    XAML_METHOD(add_collection_property, type, xaml_collection_property_info*); \
    XAML_METHOD(add_event, type, xaml_event_info*)

XAML_DECL_INTERFACE_(xaml_type_info_registration, xaml_type_info)
{
    XAML_DECL_VTBL(xaml_type_info_registration, XAML_TYPE_INFO_REGISTRATION_VTBL);

#ifdef __cplusplus
    template <typename T>
    xaml_result XAML_CALL add_attribute(T * ptr) noexcept
    {
        return add_attribute(xaml_type_guid_v<T>, (xaml_object*)ptr);
    }
#endif // __cplusplus
};

EXTERN_C XAML_META_API xaml_result XAML_CALL xaml_type_info_registration_new(xaml_guid XAML_CONST_REF, xaml_string*, xaml_string*, xaml_type_info_registration**) XAML_NOEXCEPT;

#ifdef __cplusplus
template <typename T>
xaml_result XAML_CALL xaml_type_info_registration_new(xaml_string* name, xaml_string* include_file, xaml_type_info_registration** ptr) noexcept
{
    return xaml_type_info_registration_new(xaml_type_guid_v<T>, name, include_file, ptr);
}
#endif // __cplusplus

#endif // !XAML_META_TYPE_INFO_H
