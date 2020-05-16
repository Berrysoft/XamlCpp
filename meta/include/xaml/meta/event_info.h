#ifndef XAML_META_EVENT_INFO_H
#define XAML_META_EVENT_INFO_H

#include <xaml/delegate.h>
#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_event_info, { 0x3e92f836, 0x4337, 0x40c7, { 0x83, 0x99, 0x52, 0x7e, 0x46, 0xd4, 0x30, 0x48 } })

#define XAML_EVENT_INFO_VTBL(type)                                  \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                      \
    XAML_METHOD(get_name, type, xaml_string**);                     \
    XAML_METHOD(add, type, xaml_object*, xaml_delegate*, int32_t*); \
    XAML_METHOD(remove, type, xaml_object*, int32_t)

XAML_DECL_INTERFACE_(xaml_event_info, xaml_object)
{
    XAML_DECL_VTBL(xaml_event_info, XAML_EVENT_INFO_VTBL);
};

EXTERN_C XAML_META_API xaml_result XAML_CALL xaml_event_info_new(xaml_string*, xaml_result(XAML_CALL*)(xaml_object*, xaml_delegate*, XAML_STD int32_t*) XAML_NOEXCEPT, xaml_result(XAML_CALL*)(xaml_object*, XAML_STD int32_t) XAML_NOEXCEPT, xaml_event_info**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_META_API xaml_result XAML_CALL xaml_event_info_new(xaml_string*, std::function<xaml_result(xaml_object*, xaml_delegate*, std::int32_t*)>&&, std::function<xaml_result(xaml_object*, std::int32_t)>&&, xaml_event_info**) noexcept;

template <typename T>
inline xaml_result XAML_CALL xaml_event_info_new(xaml_string* name, xaml_result (XAML_CALL T::*adder)(xaml_delegate*, std::int32_t*) noexcept, xaml_result (XAML_CALL T::*remover)(std::int32_t) noexcept, xaml_event_info** ptr) noexcept
{
    return xaml_event_info_new(
        name,
        [adder](xaml_object* target, xaml_delegate* handler, std::int32_t* ptoken) -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            return (self.get()->*adder)(handler, ptoken);
        },
        [remover](xaml_object* target, std::int32_t token) -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            return (self.get()->*remover)(token);
        },
        ptr);
}
#endif // __cplusplus

#endif // !XAML_META_EVENT_INFO_H
