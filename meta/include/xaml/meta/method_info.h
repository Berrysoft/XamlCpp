#ifndef XAML_META_METHOD_INFO_H
#define XAML_META_METHOD_INFO_H

#include <xaml/delegate.h>

XAML_CLASS(xaml_method_info, { 0xac6df520, 0x582e, 0x46e3, { 0xbd, 0x8b, 0xfb, 0x30, 0x34, 0x3e, 0x16, 0xa1 } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_method_info : xaml_delegate
{
    virtual xaml_result XAML_CALL get_name(xaml_string**) noexcept = 0;
};
#else
#define XAML_METHOD_INFO_VTBL(type) \
    XAML_DELEGATE_VTBL(type)        \
    xaml_result(XAML_CALL* get_name)(type* const, xaml_string**);

struct xaml_method_info
{
    struct
    {
        XAML_METHOD_INFO_VTBL(xaml_method_info)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_META_API xaml_result xaml_method_info_new(xaml_string*, xaml_result(XAML_CALL*)(xaml_vector_view*), xaml_method_info**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_META_API xaml_result xaml_method_info_new(xaml_string*, std::function<xaml_result(xaml_vector_view*)>&&, xaml_method_info**) noexcept;

template <typename T, typename... Args>
inline xaml_result xaml_method_info_new(xaml_string* name, xaml_result (XAML_CALL T::*func)(Args...) noexcept, xaml_method_info** ptr) noexcept
{
    if (!func) return XAML_E_INVALIDARG;
    return xaml_method_info_new(
        name,
        std::function<xaml_result(xaml_vector_view*)>{
            [func](xaml_vector_view* args) -> xaml_result {
                std::size_t size;
                XAML_RETURN_IF_FAILED(args->get_size(&size));
                if (size < sizeof...(Args)) return XAML_E_INVALIDARG;
                return __xaml_delegate_impl_invoke<xaml_result, xaml_ptr<T>, Args...>(
                    std::function<xaml_result(xaml_ptr<T>, Args...)>{ std::mem_fn(func) }, args);
            } },
        ptr);
}

#define XAML_TYPE_INFO_ADD_METHOD(name, type, method)                                                    \
    do                                                                                                   \
    {                                                                                                    \
        xaml_ptr<xaml_string> __method_name;                                                             \
        XAML_RETURN_IF_FAILED(xaml_string_new(U(#method), &__method_name));                              \
        xaml_ptr<xaml_method_info> __method_info;                                                        \
        XAML_RETURN_IF_FAILED(xaml_method_info_new(__method_name.get(), &type::method, &__method_info)); \
        XAML_RETURN_IF_FAILED(name->add_method(__method_info.get()));                                    \
    } while (0)
#endif // __cplusplus

#endif // !XAML_META_METHOD_INFO_H
