#ifndef XAML_META_META_CONTEXT_H
#define XAML_META_META_CONTEXT_H

#include <xaml/map.h>
#include <xaml/meta/module.h>
#include <xaml/meta/reflection_info.h>
#include <xaml/object.h>
#include <xaml/vector.h>

typedef enum xaml_binding_mode
{
    xaml_binding_one_time = 0x0,
    xaml_binding_one_way = 0x1,
    xaml_binding_one_way_to_source = 0x2,
    xaml_binding_two_way = xaml_binding_one_way | xaml_binding_one_way_to_source
} xaml_binding_mode;

XAML_CLASS(xaml_meta_context, { 0x8b4549b1, 0xfb13, 0x444b, { 0xa5, 0xc1, 0x5b, 0x5e, 0xa5, 0x3a, 0x02, 0xda } })

#ifdef __cplusplus
struct xaml_meta_context : xaml_object
{
    virtual xaml_result XAML_CALL get_modules(xaml_vector_view**) noexcept = 0;
    virtual xaml_result XAML_CALL add_module(xaml_module*) noexcept = 0;
    virtual xaml_result XAML_CALL get_types(xaml_map_view**) noexcept = 0;
    virtual xaml_result XAML_CALL get_type(xaml_guid const&, xaml_reflection_info**) noexcept = 0;
    virtual xaml_result XAML_CALL get_type_by_name(xaml_string*, xaml_reflection_info**) noexcept = 0;
    virtual xaml_result XAML_CALL add_type(xaml_reflection_info*) noexcept = 0;
    virtual xaml_result XAML_CALL bind(xaml_object*, xaml_string*, xaml_object*, xaml_string*, xaml_binding_mode = xaml_binding_one_time) noexcept = 0;

    template <typename T>
    xaml_result get_type(xaml_reflection_info** ptr) noexcept
    {
        return get_type(xaml_type_guid_v<T>, ptr);
    }
};
#else

#endif // __cplusplus

EXTERN_C XAML_META_API xaml_result xaml_meta_context_new(xaml_meta_context**) XAML_NOEXCEPT;

#endif // !XAML_META_META_CONTEXT_H
