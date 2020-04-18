#ifndef XAML_META_TYPE_INFO_H
#define XAML_META_TYPE_INFO_H

#include <xaml/delegate.h>
#include <xaml/map.h>
#include <xaml/meta/collection_property_info.h>
#include <xaml/meta/event_info.h>
#include <xaml/meta/property_info.h>
#include <xaml/meta/reflection_info.h>

XAML_CLASS(xaml_type_info, { 0x3de3b2c1, 0x09d6, 0x433c, { 0xbf, 0x40, 0x40, 0x2d, 0xfe, 0x28, 0xda, 0x1d } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_type_info : xaml_reflection_info
{
    virtual xaml_result XAML_CALL get_attributes(xaml_map_view**) noexcept = 0;
    virtual xaml_result XAML_CALL get_attribute(xaml_guid const&, xaml_object**) noexcept = 0;
    virtual xaml_result XAML_CALL get_constructor(xaml_delegate**) noexcept = 0;
    virtual xaml_result XAML_CALL get_methods(xaml_map_view**) noexcept = 0;
    virtual xaml_result XAML_CALL get_method(xaml_string*, xaml_delegate**) noexcept = 0;
    virtual xaml_result XAML_CALL get_properties(xaml_map_view**) noexcept = 0;
    virtual xaml_result XAML_CALL get_property(xaml_string*, xaml_property_info**) noexcept = 0;
    virtual xaml_result XAML_CALL get_collection_properties(xaml_map_view**) noexcept = 0;
    virtual xaml_result XAML_CALL get_collection_property(xaml_string*, xaml_collection_property_info**) noexcept = 0;
    virtual xaml_result XAML_CALL get_events(xaml_map_view**) noexcept = 0;
    virtual xaml_result XAML_CALL get_event(xaml_string*, xaml_event_info**) noexcept = 0;

    template <typename T>
    xaml_result get_attribute(T** ptr) noexcept
    {
        return get_attribute(xaml_type_guid_v<T>, (xaml_object**)ptr);
    }
};
#else

#endif // __cplusplus

XAML_CLASS(xaml_type_info_registration, { 0x18aecfb7, 0x7fd3, 0x44a2, { 0xba, 0xc7, 0x1b, 0x2d, 0x75, 0xb1, 0x4f, 0xc9 } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_type_info_registration : xaml_type_info
{
    virtual xaml_result XAML_CALL add_attribute(xaml_guid const&, xaml_object*) noexcept = 0;
    virtual xaml_result XAML_CALL set_constructor(xaml_delegate*) noexcept = 0;
    virtual xaml_result XAML_CALL add_method(xaml_string*, xaml_delegate*) noexcept = 0;
    virtual xaml_result XAML_CALL add_property(xaml_property_info*) noexcept = 0;
    virtual xaml_result XAML_CALL add_collection_property(xaml_collection_property_info*) noexcept = 0;
    virtual xaml_result XAML_CALL add_event(xaml_event_info*) noexcept = 0;

    template <typename T>
    xaml_result add_attribute(T* ptr) noexcept
    {
        return add_attribute(xaml_type_guid_v<T>, (xaml_object*)ptr);
    }
};
#else

#endif // __cplusplus

#endif // !XAML_META_TYPE_INFO_H
