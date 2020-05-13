#ifndef XAML_META_META_CONTEXT_H
#define XAML_META_META_CONTEXT_H

#include <xaml/map.h>
#include <xaml/meta/module.h>
#include <xaml/meta/reflection_info.h>
#include <xaml/meta/type_info.h>
#include <xaml/object.h>
#include <xaml/vector.h>

typedef enum xaml_binding_mode
{
    xaml_binding_one_time = 0x0,
    xaml_binding_one_way = 0x1,
    xaml_binding_one_way_to_source = 0x2,
    xaml_binding_two_way = xaml_binding_one_way | xaml_binding_one_way_to_source
} xaml_binding_mode;

XAML_TYPE(xaml_binding_mode, { 0x01f3d35e, 0x73cc, 0x4abd, { 0xbc, 0xa9, 0xe5, 0x07, 0x70, 0x9e, 0x06, 0x9a } })

XAML_CLASS(xaml_meta_context, { 0x8b4549b1, 0xfb13, 0x444b, { 0xa5, 0xc1, 0x5b, 0x5e, 0xa5, 0x3a, 0x02, 0xda } })

#define XAML_META_CONTEXT_VTBL(type)                                                                   \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                                                         \
    XAML_METHOD(get_modules, type, xaml_vector_view**);                                                \
    XAML_METHOD(add_module, type, xaml_module*);                                                       \
    XAML_METHOD(get_namespace, type, xaml_string*, xaml_string**);                                     \
    XAML_METHOD(add_namespace, type, xaml_string*, xaml_string*);                                      \
    XAML_METHOD(get_types, type, xaml_map_view**);                                                     \
    XAML_METHOD(get_type, type, xaml_guid XAML_CONST_REF, xaml_reflection_info**);                     \
    XAML_METHOD(get_type_by_name, type, xaml_string*, xaml_reflection_info**);                         \
    XAML_METHOD(get_type_by_namespace_name, type, xaml_string*, xaml_string*, xaml_reflection_info**); \
    XAML_METHOD(get_name_by_namespace_name, type, xaml_string*, xaml_string*, xaml_string**);          \
    XAML_METHOD(add_type, type, xaml_reflection_info*);                                                \
    XAML_METHOD(get_basic_type, type, xaml_guid XAML_CONST_REF, xaml_string**);                        \
    XAML_METHOD(add_basic_type, type, xaml_guid XAML_CONST_REF, xaml_string*);                         \
    XAML_METHOD(bind, type, xaml_object*, xaml_string*, xaml_object*, xaml_string*, xaml_binding_mode)

XAML_DECL_INTERFACE_(xaml_meta_context, xaml_object)
{
    XAML_DECL_VTBL(xaml_meta_context, XAML_META_CONTEXT_VTBL);

#ifdef __cplusplus
    template <typename T>
    xaml_result XAML_CALL get_type(xaml_reflection_info * *ptr) noexcept
    {
        return get_type(xaml_type_guid_v<T>, ptr);
    }

    template <typename T>
    xaml_result XAML_CALL get_basic_type(xaml_string * *ptr) noexcept
    {
        return get_basic_type(xaml_type_guid_v<T>, ptr);
    }

    template <typename T>
    xaml_result XAML_CALL add_basic_type(xaml_string * name) noexcept
    {
        return add_basic_type(xaml_type_guid_v<T>, name);
    }

    xaml_result XAML_CALL add_module(xaml_std_string_view_t path) noexcept
    {
        xaml_ptr<xaml_module> m;
        XAML_RETURN_IF_FAILED(xaml_module_new(&m));
        xaml_ptr<xaml_string> path_str;
        XAML_RETURN_IF_FAILED(xaml_string_new(path, &path_str));
        XAML_RETURN_IF_FAILED(m->open(path_str.get()));
        return add_module(m.get());
    }
#endif // __cplusplus
};

EXTERN_C XAML_META_API xaml_result XAML_CALL xaml_meta_context_new(xaml_meta_context**) XAML_NOEXCEPT;

#endif // !XAML_META_META_CONTEXT_H
