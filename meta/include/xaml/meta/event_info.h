#ifndef XAML_META_EVENT_INFO_H
#define XAML_META_EVENT_INFO_H

#include <xaml/meta/method_info.h>
#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_event_info, { 0x3e92f836, 0x4337, 0x40c7, { 0x83, 0x99, 0x52, 0x7e, 0x46, 0xd4, 0x30, 0x48 } })

#define XAML_EVENT_INFO_VTBL(type)                                     \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                         \
    XAML_METHOD(get_name, type, xaml_string**);                        \
    XAML_METHOD(add, type, xaml_object*, xaml_method_info*, int32_t*); \
    XAML_METHOD(remove, type, xaml_object*, int32_t)

XAML_DECL_INTERFACE_(xaml_event_info, xaml_object)
{
    XAML_DECL_VTBL(xaml_event_info, XAML_EVENT_INFO_VTBL);
};

EXTERN_C XAML_META_API xaml_result XAML_CALL xaml_event_info_new(xaml_string*, xaml_result(XAML_CALL*)(xaml_object*, xaml_method_info*, XAML_STD int32_t*) XAML_NOEXCEPT, xaml_result(XAML_CALL*)(xaml_object*, XAML_STD int32_t) XAML_NOEXCEPT, xaml_event_info**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_META_API xaml_result XAML_CALL xaml_event_info_new(xaml_string*, std::function<xaml_result(xaml_object*, xaml_method_info*, std::int32_t*)>&&, std::function<xaml_result(xaml_object*, std::int32_t)>&&, xaml_event_info**) noexcept;

template <typename T, typename TS, typename TE>
inline xaml_result XAML_CALL xaml_event_info_new(xaml_string* name, xaml_result (XAML_CALL T::*adder)(xaml_delegate<TS, TE>*, std::int32_t*) noexcept, xaml_result (XAML_CALL T::*remover)(std::int32_t) noexcept, xaml_event_info** ptr) noexcept
{
    return xaml_event_info_new(
        name,
        [adder](xaml_object* target, xaml_method_info* method, std::int32_t* ptoken) noexcept -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            xaml_ptr<xaml_delegate<TS, TE>> handler;
            XAML_RETURN_IF_FAILED((xaml_delegate_new<TS, TE>(
                [method = xaml_ptr<xaml_method_info>{ method }](xaml_interface_t<TS> sender, xaml_interface_t<TE> e) -> xaml_result {
                    xaml_ptr<xaml_vector_view<xaml_object>> args;
                    XAML_RETURN_IF_FAILED(xaml_delegate_pack_args(args, sender, e));
                    xaml_ptr<xaml_object> obj;
                    return method->invoke(args, &obj);
                },
                &handler)));
            return (self.get()->*adder)(handler, ptoken);
        },
        [remover](xaml_object* target, std::int32_t token) noexcept -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            return (self.get()->*remover)(token);
        },
        ptr);
}
#endif // __cplusplus

#endif // !XAML_META_EVENT_INFO_H
