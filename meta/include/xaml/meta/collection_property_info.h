#ifndef XAML_META_COLLECTION_PROPERTY_INFO_H
#define XAML_META_COLLECTION_PROPERTY_INFO_H

#ifdef __cplusplus
    #include <xaml/box.h>
    #include <xaml/delegate.h>
    #include <xaml/meta/conv.hpp>
    #include <xaml/ptr.hpp>
#endif // __cplusplus

#include <xaml/meta/property_info.h>
#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_collection_property_info, { 0xedd279d5, 0x1c51, 0x41c0, { 0x80, 0x6c, 0x38, 0x62, 0x9c, 0x86, 0xfd, 0x7f } })

#define XAML_COLLECTION_PROPERTY_INFO_VTBL(type)        \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));          \
    XAML_METHOD(get_name, type, xaml_string**);         \
    XAML_METHOD(get_type, type, xaml_guid*);            \
    XAML_METHOD(get_can_add, type, bool*);              \
    XAML_METHOD(get_can_remove, type, bool*);           \
    XAML_METHOD(add, type, xaml_object*, xaml_object*); \
    XAML_METHOD(remove, type, xaml_object*, xaml_object*)

XAML_DECL_INTERFACE_(xaml_collection_property_info, xaml_object)
{
    XAML_DECL_VTBL(xaml_collection_property_info, XAML_COLLECTION_PROPERTY_INFO_VTBL);
};

EXTERN_C XAML_META_API xaml_result XAML_CALL xaml_collection_property_info_new(xaml_string*, xaml_guid XAML_CONST_REF, xaml_result(XAML_CALL*)(xaml_object*, xaml_object*) XAML_NOEXCEPT, xaml_result(XAML_CALL*)(xaml_object*, xaml_object*) XAML_NOEXCEPT, xaml_collection_property_info**) XAML_NOEXCEPT;

#ifdef __cplusplus
    #ifdef XAML_SUPPORT_FUNCTION2
XAML_META_API xaml_result XAML_CALL xaml_collection_property_info_new(xaml_string*, xaml_guid const&, fu2::unique_function<xaml_result(xaml_object*, xaml_object*) noexcept>&&, fu2::unique_function<xaml_result(xaml_object*, xaml_object*) noexcept>&&, xaml_collection_property_info**) noexcept;
    #endif // XAML_SUPPORT_FUNCTION2

    #if !defined(XAML_SUPPORT_FUNCTION2) || defined(XAML_META_BUILD)
XAML_META_API xaml_result XAML_CALL xaml_collection_property_info_new(xaml_string*, xaml_guid const&, std::function<xaml_result(xaml_object*, xaml_object*)>&&, std::function<xaml_result(xaml_object*, xaml_object*)>&&, xaml_collection_property_info**) noexcept;
    #endif

template <typename TValue, typename T, typename TValueAdd, typename TValueRemove = TValueAdd>
inline xaml_result XAML_CALL __xaml_collection_property_info_new(xaml_string* name, xaml_result (XAML_CALL T::*adder)(TValueAdd) noexcept, xaml_result (XAML_CALL T::*remover)(TValueRemove) noexcept, xaml_collection_property_info** ptr) noexcept
{
    return xaml_collection_property_info_new(
        name, xaml_type_guid_v<TValue>,
        __xaml_unique_function_wrapper_t<xaml_result(xaml_object*, xaml_object*) noexcept>(
            [adder](xaml_object* target, xaml_object* obj) noexcept -> xaml_result {
                xaml_ptr<T> self;
                XAML_RETURN_IF_FAILED(target->query(&self));
                __xaml_wrapper_t<std::decay_t<TValueAdd>> value;
                XAML_RETURN_IF_FAILED(__xaml_converter<__xaml_wrapper_t<std::decay_t<TValueAdd>>>{}(obj, &value));
                return (self.get()->*adder)(value);
            }),
        __xaml_unique_function_wrapper_t<xaml_result(xaml_object*, xaml_object*) noexcept>(
            [remover](xaml_object* target, xaml_object* obj) noexcept -> xaml_result {
                xaml_ptr<T> self;
                XAML_RETURN_IF_FAILED(target->query(&self));
                __xaml_wrapper_t<std::decay_t<TValueRemove>> value;
                XAML_RETURN_IF_FAILED(__xaml_converter<__xaml_wrapper_t<std::decay_t<TValueRemove>>>{}(obj, &value));
                return (self.get()->*remover)(value);
            }),
        ptr);
}
#endif // __cplusplus

#endif // !XAML_META_COLLECTION_PROPERTY_INFO_H
