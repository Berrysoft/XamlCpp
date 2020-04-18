#ifndef XAML_META_EVENT_INFO_H
#define XAML_META_EVENT_INFO_H

#include <xaml/delegate.h>
#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_event_info, { 0x3e92f836, 0x4337, 0x40c7, { 0x83, 0x99, 0x52, 0x7e, 0x46, 0xd4, 0x30, 0x48 } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_event_info : xaml_object
{
    virtual xaml_result XAML_CALL get_name(xaml_string**) noexcept = 0;
    virtual xaml_result XAML_CALL add(xaml_object*, xaml_delegate*, std::size_t*) noexcept = 0;
    virtual xaml_result XAML_CALL remove(xaml_object*, std::size_t) noexcept = 0;
};
#else
#define XAML_EVENT_VTBL(type)                                                        \
    xaml_result(XAML_CALL* get_name)(type* const, xaml_string**);                    \
    xaml_result(XAML_CALL* add)(type* const, xaml_object*, xaml_delegate*, size_t*); \
    xaml_result(XAML_CALL* remove)(type* const, xaml_object*, size_t);

struct xaml_event_info
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_event_info)
        XAML_EVENT_VTBL(xaml_Event_info)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_META_API xaml_result xaml_event_info_new(xaml_string*, xaml_result(XAML_CALL*)(xaml_object*, xaml_delegate*, XAML_CSTD size_t*), xaml_result(XAML_CALL*)(xaml_object*, XAML_CSTD size_t), xaml_event_info**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_META_API xaml_result xaml_event_info_new(xaml_string*, std::function<xaml_result(xaml_object*, xaml_delegate*, std::size_t*)>&&, std::function<xaml_result(xaml_object*, std::size_t)>&&, xaml_event_info**) noexcept;

template <typename T>
inline xaml_result xaml_event_info_new(xaml_string* name, xaml_result (T::*XAML_CALL adder)(xaml_delegate*, std::size_t*), xaml_result (T::*XAML_CALL remover)(std::size_t), xaml_event_info** ptr) noexcept
{
    return xaml_event_info_new(
        name,
        [adder](xaml_object* target, xaml_delegate* handler, std::size_t* ptoken) -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            return self.get()->*adder(handler, ptoken);
        },
        [remover](xaml_object* target, std::size_t token) -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            return self.get()->*remover(token);
        },
        ptr);
}
#endif // __cplusplus

#endif // !XAML_META_EVENT_INFO_H
