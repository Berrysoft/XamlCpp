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

#ifdef __cplusplus
struct XAML_NOVTBL xaml_type_info : xaml_reflection_info
{
    virtual xaml_result XAML_CALL get_attributes(xaml_map_view**) noexcept = 0;
    virtual xaml_result XAML_CALL get_attribute(xaml_guid const&, xaml_object**) noexcept = 0;
    virtual xaml_result XAML_CALL get_constructor(xaml_delegate**) noexcept = 0;
    virtual xaml_result XAML_CALL get_methods(xaml_map_view**) noexcept = 0;
    virtual xaml_result XAML_CALL get_method(xaml_string*, xaml_method_info**) noexcept = 0;
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

    xaml_result construct(xaml_object** ptr) noexcept
    {
        xaml_ptr<xaml_delegate> ctor;
        XAML_RETURN_IF_FAILED(get_constructor(&ctor));
        if (!ctor) return XAML_E_NOTIMPL;
        xaml_ptr<xaml_vector> args;
        XAML_RETURN_IF_FAILED(xaml_vector_new(&args));
        return ctor->invoke(args.get(), ptr);
    }
};
#else
#define XAML_TYPE_INFO_VTBL(type)                                                                                \
    XAML_REFLECTION_INFO_VTBL(type)                                                                              \
    xaml_result(XAML_CALL* get_attributes)(type* const, xaml_map_view**);                                        \
    xaml_result(XAML_CALL* get_attribute)(type* const, xaml_guid const*, xaml_object**);                         \
    xaml_result(XAML_CALL* get_constructor)(type* const, xaml_delegate**);                                       \
    xaml_result(XAML_CALL* get_methods)(type* const, xaml_map_view**);                                           \
    xaml_result(XAML_CALL* get_method)(type* const, xaml_string*, xaml_delegate**);                              \
    xaml_result(XAML_CALL* get_properties)(type* const, xaml_map_view**);                                        \
    xaml_result(XAML_CALL* get_property)(type* const, xaml_string*, xaml_property_info**);                       \
    xaml_result(XAML_CALL* get_collection_properties)(type* const, xaml_map_view**);                             \
    xaml_result(XAML_CALL* get_collection_property)(type* const, xaml_string*, xaml_collection_property_info**); \
    xaml_result(XAML_CALL* get_events)(type* const, xaml_map_view**);                                            \
    xaml_result(XAML_CALL* get_event)(type* const, xaml_string*, xaml_event_info**);

struct xaml_type_info
{
    struct
    {
        XAML_TYPE_INFO_VTBL(xaml_type_info)
    } const* vtbl;
};
#endif // __cplusplus

XAML_CLASS(xaml_type_info_registration, { 0x18aecfb7, 0x7fd3, 0x44a2, { 0xba, 0xc7, 0x1b, 0x2d, 0x75, 0xb1, 0x4f, 0xc9 } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_type_info_registration : xaml_type_info
{
    virtual xaml_result XAML_CALL add_attribute(xaml_guid const&, xaml_object*) noexcept = 0;
    virtual xaml_result XAML_CALL set_constructor(xaml_delegate*) noexcept = 0;
    virtual xaml_result XAML_CALL add_method(xaml_method_info*) noexcept = 0;
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
#define XAML_TYPE_INFO_REGISTRATION_VTBL(type)                                                    \
    XAML_TYPE_INFO_VTBL(type)                                                                     \
    xaml_result(XAML_CALL* add_attribute)(type* const, xaml_guid const*, xaml_object*);           \
    xaml_result(XAML_CALL* set_constructor)(type* const, xaml_delegate*);                         \
    xaml_result(XAML_CALL* add_method)(type* const, xaml_string*, xaml_delegate*);                \
    xaml_result(XAML_CALL* add_property)(type* const, xaml_property_info*);                       \
    xaml_result(XAML_CALL* add_collection_property)(type* const, xaml_collection_property_info*); \
    xaml_result(XAML_CALL* add_event)(type* const, xaml_event_info*);

struct xaml_type_info_registration
{
    struct
    {
        XAML_TYPE_INFO_REGISTRATION_VTBL(xaml_type_info_registration)
    } const* vtbl;
};
#endif // __cplusplus

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
