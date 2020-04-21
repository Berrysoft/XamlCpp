#ifndef XAML_META_TYPE_INFO_H
#define XAML_META_TYPE_INFO_H

#include <xaml/delegate.h>
#include <xaml/map.h>
#include <xaml/meta/collection_property_info.h>
#include <xaml/meta/event_info.h>
#include <xaml/meta/method_info.h>
#include <xaml/meta/property_info.h>
#include <xaml/meta/reflection_info.h>

XAML_CLASS(xaml_type_info, { 0x3de3b2c1, 0x09d6, 0x433c, { 0xbf, 0x40, 0x40, 0x2d, 0xfe, 0x28, 0xda, 0x1d } })

#define XAML_TYPE_INFO_VTBL(type)                                                              \
    XAML_VTBL_INHERIT(XAML_REFLECTION_INFO_VTBL(type));                                        \
    XAML_METHOD(get_attributes, type, xaml_map_view**);                                        \
    XAML_METHOD(get_attribute, type, xaml_guid XAML_CONST_REF, xaml_object**);                 \
    XAML_METHOD(get_constructor, type, xaml_delegate**);                                       \
    XAML_METHOD(get_methods, type, xaml_map_view**);                                           \
    XAML_METHOD(get_method, type, xaml_string*, xaml_method_info**);                           \
    XAML_METHOD(get_properties, type, xaml_map_view**);                                        \
    XAML_METHOD(get_property, type, xaml_string*, xaml_property_info**);                       \
    XAML_METHOD(get_collection_properties, type, xaml_map_view**);                             \
    XAML_METHOD(get_collection_property, type, xaml_string*, xaml_collection_property_info**); \
    XAML_METHOD(get_events, type, xaml_map_view**);                                            \
    XAML_METHOD(get_event, type, xaml_string*, xaml_event_info**)

XAML_DECL_INTERFACE_(xaml_type_info, xaml_reflection_info)
{
    XAML_DECL_VTBL(xaml_type_info, XAML_TYPE_INFO_VTBL);

#ifdef __cplusplus
    template <typename T>
    xaml_result get_attribute(T * *ptr) noexcept
    {
        return get_attribute(xaml_type_guid_v<T>, (xaml_object**)ptr);
    }

    xaml_result construct(xaml_object * *ptr) noexcept
    {
        xaml_ptr<xaml_delegate> ctor;
        XAML_RETURN_IF_FAILED(get_constructor(&ctor));
        if (!ctor) return XAML_E_NOTIMPL;
        xaml_ptr<xaml_vector> args;
        XAML_RETURN_IF_FAILED(xaml_vector_new(&args));
        return ctor->invoke(args.get(), ptr);
    }
#endif // __cplusplus
};

XAML_CLASS(xaml_type_info_registration, { 0x18aecfb7, 0x7fd3, 0x44a2, { 0xba, 0xc7, 0x1b, 0x2d, 0x75, 0xb1, 0x4f, 0xc9 } })

#define XAML_TYPE_INFO_REGISTRATION_VTBL(type)                                  \
    XAML_VTBL_INHERIT(XAML_TYPE_INFO_VTBL(type));                               \
    XAML_METHOD(add_attribute, type, xaml_guid XAML_CONST_REF, xaml_object*);   \
    XAML_METHOD(set_constructor, type, xaml_delegate*);                         \
    XAML_METHOD(add_method, type, xaml_method_info*);                           \
    XAML_METHOD(add_property, type, xaml_property_info*);                       \
    XAML_METHOD(add_collection_property, type, xaml_collection_property_info*); \
    XAML_METHOD(add_event, type, xaml_event_info*)

XAML_DECL_INTERFACE_(xaml_type_info_registration, xaml_type_info)
{
    XAML_DECL_VTBL(xaml_type_info_registration, XAML_TYPE_INFO_REGISTRATION_VTBL);

#ifdef __cplusplus
    template <typename T>
    xaml_result add_attribute(T * ptr) noexcept
    {
        return add_attribute(xaml_type_guid_v<T>, (xaml_object*)ptr);
    }
#endif // __cplusplus
};

EXTERN_C XAML_META_API xaml_result xaml_type_info_registration_new(xaml_guid XAML_CONST_REF, xaml_string*, xaml_string*, xaml_type_info_registration**) XAML_NOEXCEPT;

#ifdef __cplusplus
template <typename T>
xaml_result xaml_type_info_registration_new(xaml_string* name, xaml_string* include_file, xaml_type_info_registration** ptr) noexcept
{
    return xaml_type_info_registration_new(xaml_type_guid_v<T>, name, include_file, ptr);
}

#define XAML_TYPE_INFO_NEW(name, type, file)                          \
    xaml_ptr<xaml_string> __type_name;                                \
    XAML_RETURN_IF_FAILED(xaml_string_new(U(#type), &__type_name));   \
    xaml_ptr<xaml_string> __include_file;                             \
    XAML_RETURN_IF_FAILED(xaml_string_new(U(file), &__include_file)); \
    xaml_ptr<xaml_type_info_registration> name;                       \
    XAML_RETURN_IF_FAILED(xaml_type_info_registration_new<type>(__type_name.get(), __include_file.get(), &name))

#define XAML_TYPE_INFO_ADD_CTOR(name, type)                      \
    do                                                           \
    {                                                            \
        xaml_ptr<xaml_delegate> __ctor;                          \
        XAML_RETURN_IF_FAILED(xaml_delegate_new<xaml_ptr<type>>( \
            []() -> xaml_ptr<type> {                             \
                xaml_ptr<type> __res;                            \
                XAML_THROW_IF_FAILED(type##_new(&__res));        \
                return __res;                                    \
            },                                                   \
            &__ctor));                                           \
        name->set_constructor(__ctor.get());                     \
    } while (0)
#endif // __cplusplus

#endif // !XAML_META_TYPE_INFO_H
