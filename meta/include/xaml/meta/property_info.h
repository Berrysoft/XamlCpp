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

#ifdef __cplusplus
struct XAML_NOVTBL xaml_property_info : xaml_object
{
    virtual xaml_result XAML_CALL get_name(xaml_string**) noexcept = 0;
    virtual xaml_result XAML_CALL get_type(xaml_guid*) noexcept = 0;
    virtual xaml_result XAML_CALL get_can_read(bool*) noexcept = 0;
    virtual xaml_result XAML_CALL get_can_write(bool*) noexcept = 0;
    virtual xaml_result XAML_CALL get(xaml_object*, xaml_object**) noexcept = 0;
    virtual xaml_result XAML_CALL set(xaml_object*, xaml_object*) noexcept = 0;
};
#else
#define XAML_PROPERTY_INFO_VTBL(type)                                      \
    XAML_OBJECT_VTBL(type)                                                 \
    xaml_result(XAML_CALL* get_name)(type* const, xaml_string**);          \
    xaml_result(XAML_CALL* get_type)(type* const, xaml_guid*);             \
    xaml_result(XAML_CALL* get_can_read)(type* const, bool*);              \
    xaml_result(XAML_CALL* get_can_write)(type* const, bool*);             \
    xaml_result(XAML_CALL* get)(type* const, xaml_object*, xaml_object**); \
    xaml_result(XAML_CALL* set)(type* const, xaml_object*, xaml_object*);

struct xaml_property_info
{
    struct
    {
        XAML_PROPERTY_INFO_VTBL(xaml_property_info)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_META_API xaml_result xaml_property_info_new(xaml_string*, xaml_guid XAML_CONST_REF, xaml_result(XAML_CALL*)(xaml_object*, xaml_object**), xaml_result(XAML_CALL*)(xaml_object*, xaml_object*), xaml_property_info**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_META_API xaml_result xaml_property_info_new(xaml_string*, xaml_guid const&, std::function<xaml_result(xaml_object*, xaml_object**)>&&, std::function<xaml_result(xaml_object*, xaml_object*)>&&, xaml_property_info**) noexcept;

template <typename T, typename TValueGet, typename TValueSet = TValueGet>
xaml_result xaml_property_info_new(xaml_string* name, xaml_result (XAML_CALL T::*getter)(TValueGet*) noexcept, xaml_result (XAML_CALL T::*setter)(TValueSet) noexcept, xaml_property_info** ptr) noexcept
{
    return xaml_property_info_new(
        name, xaml_type_guid_v<T>,
        [getter](xaml_object* target, xaml_object** ptr) -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            std::decay_t<TValueGet> value;
            XAML_RETURN_IF_FAILED((self.get()->*getter)(&value));
            return box_value(value, ptr);
        },
        [setter](xaml_object* target, xaml_object* obj) -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            std::decay_t<TValueSet> value;
            XAML_RETURN_IF_FAILED(__xaml_converter<std::decay_t<TValueSet>>{}(obj, value));
            return (self.get()->*setter)(value);
        },
        ptr);
}

#define XAML_TYPE_INFO_ADD_PROP(name, type, prop)                                                                             \
    do                                                                                                                        \
    {                                                                                                                         \
        xaml_ptr<xaml_string> __prop_name;                                                                                    \
        XAML_RETURN_IF_FAILED(xaml_string_new(U(#prop), &__prop_name));                                                       \
        xaml_ptr<xaml_property_info> __prop_info;                                                                             \
        XAML_RETURN_IF_FAILED(xaml_property_info_new(__prop_name.get(), &type::get_##prop, &type::set_##prop, &__prop_info)); \
        XAML_RETURN_IF_FAILED(info->add_property(__prop_info.get()));                                                         \
    } while (0)
#endif // __cplusplus

#endif // !XAML_META_PROPERTY_INFO_H
