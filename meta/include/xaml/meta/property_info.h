#ifndef XAML_META_PROPERTY_INFO_H
#define XAML_META_PROPERTY_INFO_H

#ifdef __cplusplus
#include <functional>
#include <xaml/box.h>
#include <xaml/meta/conv.hpp>
#include <xaml/ptr.hpp>
#else
#include <stdbool.h>
#endif // __cplusplus

#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_property_info, { 0x80cafe4e, 0xcda3, 0x476b, { 0xa5, 0xe3, 0x2e, 0x80, 0xa1, 0x11, 0xba, 0x85 } })

#define XAML_PROPERTY_INFO_VTBL(type)                    \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));           \
    XAML_METHOD(get_name, type, xaml_string**);          \
    XAML_METHOD(get_type, type, xaml_guid*);             \
    XAML_METHOD(get_can_read, type, bool*);              \
    XAML_METHOD(get_can_write, type, bool*);             \
    XAML_METHOD(get, type, xaml_object*, xaml_object**); \
    XAML_METHOD(set, type, xaml_object*, xaml_object*)

XAML_DECL_INTERFACE_(xaml_property_info, xaml_object)
{
    XAML_DECL_VTBL(xaml_property_info, XAML_PROPERTY_INFO_VTBL);
};

EXTERN_C XAML_META_API xaml_result xaml_property_info_new(xaml_string*, xaml_guid XAML_CONST_REF, xaml_result(XAML_CALL*)(xaml_object*, xaml_object**), xaml_result(XAML_CALL*)(xaml_object*, xaml_object*), xaml_property_info**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_META_API xaml_result xaml_property_info_new(xaml_string*, xaml_guid const&, std::function<xaml_result(xaml_object*, xaml_object**)>&&, std::function<xaml_result(xaml_object*, xaml_object*)>&&, xaml_property_info**) noexcept;

template <typename T, typename = void>
struct __xaml_wrapper
{
    using type = T;
    static T get(T const& value) noexcept { return value; }
};

template <typename T>
struct __xaml_wrapper<T*, std::enable_if_t<std::is_base_of_v<xaml_object, T>>>
{
    using type = xaml_ptr<T>;
    static T* get(xaml_ptr<T> const& value) noexcept { return value.get(); }
};

template <typename T>
using __xaml_wrapper_t = typename __xaml_wrapper<T>::type;

template <typename T>
decltype(auto) __xaml_wrapper_get(__xaml_wrapper_t<T> const& value) noexcept
{
    return __xaml_wrapper<T>::get(value);
}

template <typename T, typename TValueGet, typename TValueSet = TValueGet>
xaml_result xaml_property_info_new(xaml_string* name, xaml_result (XAML_CALL T::*getter)(TValueGet*) noexcept, xaml_result (XAML_CALL T::*setter)(TValueSet) noexcept, xaml_property_info** ptr) noexcept
{
    return xaml_property_info_new(
        name, xaml_type_guid_v<T>,
        [getter](xaml_object* target, xaml_object** ptr) -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            __xaml_wrapper_t<std::decay_t<TValueGet>> value;
            XAML_RETURN_IF_FAILED((self.get()->*getter)(&value));
            return xaml_box_value(__xaml_wrapper_get<std::decay_t<TValueGet>>(value), ptr);
        },
        [setter](xaml_object* target, xaml_object* obj) -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            __xaml_wrapper_t<std::decay_t<TValueSet>> value;
            XAML_RETURN_IF_FAILED(__xaml_converter<__xaml_wrapper_t<std::decay_t<TValueSet>>>{}(obj, value));
            return (self.get()->*setter)(__xaml_wrapper_get<std::decay_t<TValueSet>>(value));
        },
        ptr);
}

template <typename T, typename TValueGet>
xaml_result xaml_property_info_new(xaml_string* name, xaml_result (XAML_CALL T::*getter)(TValueGet*) noexcept, xaml_property_info** ptr) noexcept
{
    return xaml_property_info_new(
        name, xaml_type_guid_v<T>,
        [getter](xaml_object* target, xaml_object** ptr) -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            __xaml_wrapper_t<std::decay_t<TValueGet>> value;
            XAML_RETURN_IF_FAILED((self.get()->*getter)(&value));
            return xaml_box_value(__xaml_wrapper_get<std::decay_t<TValueGet>>(value), ptr);
        },
        nullptr, ptr);
}
#endif // __cplusplus

#endif // !XAML_META_PROPERTY_INFO_H
