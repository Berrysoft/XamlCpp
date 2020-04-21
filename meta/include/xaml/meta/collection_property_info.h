#ifndef XAML_META_COLLECTION_PROPERTY_INFO_H
#define XAML_META_COLLECTION_PROPERTY_INFO_H

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

EXTERN_C XAML_META_API xaml_result xaml_collection_property_info_new(xaml_string*, xaml_guid XAML_CONST_REF, xaml_result(XAML_CALL*)(xaml_object*, xaml_object*), xaml_result(XAML_CALL*)(xaml_object*, xaml_object*), xaml_collection_property_info**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_META_API xaml_result xaml_collection_property_info_new(xaml_string*, xaml_guid const&, std::function<xaml_result(xaml_object*, xaml_object*)>&&, std::function<xaml_result(xaml_object*, xaml_object*)>&&, xaml_collection_property_info**) noexcept;

template <typename T, typename TValueAdd, typename TValueRemove = TValueAdd>
inline xaml_result xaml_collection_property_info_new(xaml_string* name, xaml_result (XAML_CALL T::*adder)(TValueAdd) noexcept, xaml_result (XAML_CALL T::*remover)(TValueRemove) noexcept, xaml_collection_property_info** ptr) noexcept
{
    return xaml_collection_property_info_new(
        name, xaml_type_guid_v<T>,
        [adder](xaml_object* target, xaml_object* obj) -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            std::decay_t<TValueAdd> value;
            XAML_RETURN_IF_FAILED(__xaml_converter<std::decay_t<TValueAdd>>{}(obj, value));
            return (self.get()->*adder)(value);
        },
        [remover](xaml_object* target, xaml_object* obj) -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            std::decay_t<TValueRemove> value;
            XAML_RETURN_IF_FAILED(__xaml_converter<std::decay_t<TValueRemove>>{}(obj, value));
            return (self.get()->*remover)(value);
        },
        ptr);
}

#define XAML_TYPE_INFO_ADD_CPROP(name, type, prop)                                                                               \
    do                                                                                                                           \
    {                                                                                                                            \
        xaml_ptr<xaml_string> __prop_name;                                                                                       \
        XAML_RETURN_IF_FAILED(xaml_string_new(U(#prop), &__prop_name));                                                          \
        xaml_ptr<xaml_property_info> __prop_info;                                                                                \
        XAML_RETURN_IF_FAILED(xaml_property_info_new(__prop_name.get(), &type::add_##prop, &type::remove_##prop, &__prop_info)); \
        XAML_RETURN_IF_FAILED(info->add_collection_property(__prop_info.get()));                                                 \
    } while (0)

#endif // __cplusplus

#endif // !XAML_META_COLLECTION_PROPERTY_INFO_H
