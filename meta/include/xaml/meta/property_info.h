#ifndef XAML_META_PROPERTY_INFO_H
#define XAML_META_PROPERTY_INFO_H

#ifdef __cplusplus
#include <functional>
#include <xaml/box.h>
#include <xaml/xaml_ptr.hpp>
#else
#include <stdbool.h>
#endif // __cplusplus

#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_property_info, { 0x80cafe4e, 0xcda3, 0x476b, { 0xa5, 0xe3, 0x2e, 0x80, 0xa1, 0x11, 0xba, 0x85 } })

#ifdef __cplusplus
struct xaml_property_info : xaml_object
{
    virtual xaml_result XAML_CALL get_name(xaml_string**) const noexcept = 0;
    virtual xaml_result XAML_CALL get_type(xaml_guid*) const noexcept = 0;
    virtual xaml_result XAML_CALL get_can_read(bool*) const noexcept = 0;
    virtual xaml_result XAML_CALL get_can_write(bool*) const noexcept = 0;
    virtual xaml_result XAML_CALL get(xaml_object*, xaml_object**) const noexcept = 0;
    virtual xaml_result XAML_CALL set(xaml_object*, xaml_object*) const noexcept = 0;
};
#else
#define XAML_PROPERTY_INFO_VTBL(type)                                            \
    xaml_result(XAML_CALL* get_name)(type const* const, xaml_string**);          \
    xaml_result(XAML_CALL* get_type)(type const* const, xaml_guid*);             \
    xaml_result(XAML_CALL* get_can_read)(type const* const, bool*);              \
    xaml_result(XAML_CALL* get_can_write)(type const* const, bool*);             \
    xaml_result(XAML_CALL* get)(type const* const, xaml_object*, xaml_object**); \
    xaml_result(XAML_CALL* set)(type const* const, xaml_object*, xaml_object*);

struct xaml_property_info
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_property_info)
        XAML_PROPERTY_INFO_VTBL(xaml_property_info)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_META_API xaml_result xaml_property_info_new(xaml_string*, xaml_guid XAML_CONST_REF, xaml_result(XAML_CALL*)(xaml_object*, xaml_object**), xaml_result(XAML_CALL*)(xaml_object*, xaml_object*), xaml_property_info**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_META_API xaml_result xaml_property_info_new(xaml_string*, xaml_guid const&, std::function<xaml_result XAML_CALL(xaml_object*, xaml_object**)>&&, std::function<xaml_result XAML_CALL(xaml_object*, xaml_object*)>&&, xaml_property_info**) noexcept;

template <typename T, typename TValueGet, typename TValueSet = TValueGet>
xaml_result xaml_property_info_new(xaml_string* name, xaml_result (T::*XAML_CALL getter)(TValueGet*), xaml_result (T::*XAML_CALL setter)(TValueSet), xaml_property_info**) noexcept
{
    return xaml_property_info_new(
        name, xaml_type_guid_v<T>,
        [getter] XAML_CALL(xaml_object * target, xaml_object * *ptr) -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            TValueGet value;
            XAML_RETURN_IF_FAILED(self.get()->*getter(&value));
            return box_value(value, ptr);
        },
        [setter] XAML_CALL(xaml_object * target, xaml_object * obj) -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            TValueSet value;
            XAML_RETURN_IF_FAILED(unbox_value(obj, value));
            return self.get()->*setter(value);
        },
        ptr);
}
#endif // __cplusplus

#endif // !XAML_META_PROPERTY_INFO_H
