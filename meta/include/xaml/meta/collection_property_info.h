#ifndef XAML_META_COLLECTION_PROPERTY_INFO_H
#define XAML_META_COLLECTION_PROPERTY_INFO_H

#ifdef __cplusplus
#include <functional>
#include <xaml/box.h>
#include <xaml/xaml_ptr.hpp>
#else
#include <stdbool.h>
#endif // __cplusplus

#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_collection_property_info, { 0xedd279d5, 0x1c51, 0x41c0, { 0x80, 0x6c, 0x38, 0x62, 0x9c, 0x86, 0xfd, 0x7f } })

#ifdef __cplusplus
struct xaml_collection_property_info : xaml_object
{
    virtual xaml_result XAML_CALL get_name(xaml_string**) const noexcept = 0;
    virtual xaml_result XAML_CALL get_type(xaml_guid*) const noexcept = 0;
    virtual xaml_result XAML_CALL get_can_add(bool*) const noexcept = 0;
    virtual xaml_result XAML_CALL get_can_remove(bool*) const noexcept = 0;
    virtual xaml_result XAML_CALL add(xaml_object*, xaml_object*) const noexcept = 0;
    virtual xaml_result XAML_CALL remove(xaml_object*, xaml_object*) const noexcept = 0;
};
#else
#define XAML_COLLECTION_PROPERTY_INFO_VTBL(type)             \
    xaml_result(XAML_CALL* get_name)(xaml_string**);         \
    xaml_result(XAML_CALL* get_type)(xaml_guid*);            \
    xaml_result(XAML_CALL* get_can_add)(bool*);              \
    xaml_result(XAML_CALL* get_can_remove)(bool*);           \
    xaml_result(XAML_CALL* add)(xaml_object*, xaml_object*); \
    xaml_result(XAML_CALL* remove)(xaml_object*, xaml_object*);

struct xaml_collection_property_info
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_collection_property_info)
        XAML_COLLECTION_PROPERTY_INFO_VTBL(xaml_collection_property_info)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_META_API xaml_result xaml_collection_property_info_new(xaml_string*, xaml_guid XAML_CONST_REF, xaml_result(XAML_CALL*)(xaml_object*, xaml_object*), xaml_result(XAML_CALL*)(xaml_object*, xaml_object*), xaml_collection_property_info**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_META_API xaml_result xaml_collection_property_info_new(xaml_string*, xaml_guid const&, std::function<xaml_result XAML_CALL(xaml_object*, xaml_object*)>&&, std::function<xaml_result XAML_CALL(xaml_object*, xaml_object*)>&&, xaml_collection_property_info**) noexcept;

template <typename T, typename TValueAdd, typename TValueRemove = TValueAdd>
inline xaml_result xaml_collection_property_info_new(xaml_string*, xaml_result (T::*XAML_CALL adder)(TValueAdd), xaml_result (T::*XAML_CALL remover)(TValueRemove), xaml_collection_property_info** ptr) noexcept
{
    return xaml_collection_property_info_new(
        name, xaml_type_guid_v<T>,
        [adder] XAML_CALL(xaml_object * target, xaml_object * obj) -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            TValueAdd value;
            XAML_RETURN_IF_FAILED(unbox_value(obj, value));
            return self.get()->*adder(value);
        },
        [remover] XAML_CALL(xaml_object * target, xaml_object * obj) -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            TValueSet value;
            XAML_RETURN_IF_FAILED(unbox_value(obj, value));
            return self.get()->*remover(value);
        },
        ptr);
}
#endif // __cplusplus

#endif // !XAML_META_COLLECTION_PROPERTY_INFO_H
