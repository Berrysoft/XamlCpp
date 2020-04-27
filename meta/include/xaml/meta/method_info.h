#ifndef XAML_META_METHOD_INFO_H
#define XAML_META_METHOD_INFO_H

#include <xaml/delegate.h>

XAML_CLASS(xaml_method_info, { 0xac6df520, 0x582e, 0x46e3, { 0xbd, 0x8b, 0xfb, 0x30, 0x34, 0x3e, 0x16, 0xa1 } })

#define XAML_METHOD_INFO_VTBL(type)              \
    XAML_VTBL_INHERIT(XAML_DELEGATE_VTBL(type)); \
    XAML_METHOD(get_name, type, xaml_string**)

XAML_DECL_INTERFACE_(xaml_method_info, xaml_delegate)
{
    XAML_DECL_VTBL(xaml_method_info, XAML_METHOD_INFO_VTBL);
};

EXTERN_C XAML_META_API xaml_result XAML_CALL xaml_method_info_new(xaml_string*, xaml_result(XAML_CALL*)(xaml_vector_view*) XAML_NOEXCEPT, xaml_method_info**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_META_API xaml_result XAML_CALL xaml_method_info_new(xaml_string*, std::function<xaml_result(xaml_vector_view*)>&&, xaml_method_info**) noexcept;

template <typename T, typename... Args>
inline xaml_result XAML_CALL xaml_method_info_new(xaml_string* name, xaml_result (XAML_CALL T::*func)(Args...) noexcept, xaml_method_info** ptr) noexcept
{
    if (!func) return XAML_E_INVALIDARG;
    return xaml_method_info_new(
        name,
        std::function<xaml_result(xaml_vector_view*)>{
            [func](xaml_vector_view* args) -> xaml_result {
                std::int32_t size;
                XAML_RETURN_IF_FAILED(args->get_size(&size));
                if (size < sizeof...(Args)) return XAML_E_INVALIDARG;
                return __xaml_delegate_impl_invoke<xaml_result, xaml_ptr<T>, Args...>(
                    std::function<xaml_result(xaml_ptr<T>, Args...)>{ std::mem_fn(func) }, args);
            } },
        ptr);
}
#endif // __cplusplus

#endif // !XAML_META_METHOD_INFO_H
