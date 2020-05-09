#ifndef XAML_DELEGATE_H
#define XAML_DELEGATE_H

#ifdef __cplusplus
#include <functional>
#include <xaml/box.h>
#include <xaml/ptr.hpp>
#endif // __cplusplus

#include <xaml/object.h>
#include <xaml/vector.h>

XAML_CLASS(xaml_delegate, { 0x3a1a793a, 0x8a83, 0x4d40, { 0x89, 0x62, 0x03, 0x9e, 0x00, 0x0c, 0x0f, 0xc3 } })

#define XAML_DELEGATE_VTBL(type)               \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(invoke, type, xaml_vector_view*, xaml_object**)

XAML_DECL_INTERFACE_(xaml_delegate, xaml_object)
{
    XAML_DECL_VTBL(xaml_delegate, XAML_DELEGATE_VTBL);
};

EXTERN_C XAML_API xaml_result XAML_CALL xaml_delegate_new(xaml_result(XAML_CALL*)(xaml_vector_view*, xaml_object**) XAML_NOEXCEPT, xaml_delegate**) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result XAML_CALL xaml_delegate_bind(xaml_delegate*, xaml_vector_view*, xaml_delegate**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_API xaml_result XAML_CALL xaml_delegate_new(std::function<xaml_result(xaml_vector_view*, xaml_object**)>&&, xaml_delegate**) noexcept;

inline xaml_ptr<xaml_object> XAML_CALL __xaml_delegate_impl_invoke_impl_get_at(xaml_vector_view* args, int32_t index)
{
    xaml_ptr<xaml_object> item;
    XAML_THROW_IF_FAILED(args->get_at(index, &item));
    return item;
}

template <typename Return, typename... Args, std::int32_t... Indicies>
Return XAML_CALL __xaml_delegate_impl_invoke_impl(std::function<Return(Args...)> const& func, xaml_vector_view* args, std::integer_sequence<std::int32_t, Indicies...>)
{
    return func(xaml_unbox_value<Args>(__xaml_delegate_impl_invoke_impl_get_at(args, Indicies))...);
}

template <typename Return, typename... Args>
Return XAML_CALL __xaml_delegate_impl_invoke(std::function<Return(Args...)> const& func, xaml_vector_view* args)
{
    return __xaml_delegate_impl_invoke_impl<Return, Args...>(func, args, std::make_integer_sequence<std::int32_t, sizeof...(Args)>{});
}

template <typename Return, typename... Args, typename F>
inline xaml_result XAML_CALL xaml_delegate_new(F&& func, xaml_delegate** ptr) noexcept
{
    return xaml_delegate_new(
        std::function<xaml_result(xaml_vector_view*, xaml_object**)>{
            [func = std::function<Return(Args...)>(std::forward<F>(func))](xaml_vector_view* args, xaml_object** ptr) -> xaml_result {
                std::int32_t size;
                XAML_RETURN_IF_FAILED(args->get_size(&size));
                if (size < sizeof...(Args)) return XAML_E_INVALIDARG;
                try
                {
                    if constexpr (std::is_same_v<Return, void>)
                    {
                        __xaml_delegate_impl_invoke<Return, Args...>(func, args);
                        *ptr = nullptr;
                        return XAML_S_OK;
                    }
                    else
                    {
                        Return res = __xaml_delegate_impl_invoke<Return, Args...>(func, args);
                        return xaml_box_value(res, ptr);
                    }
                }
                XAML_CATCH_RETURN() } },
        ptr);
}

template <typename Return, typename... Args, typename F>
Return XAML_CALL __xaml_delegate_noexcept_invoke(F&& f, xaml_result* pres, Args&&... args) noexcept
{
    Return result;
    xaml_result hr = f(std::forward<Args>(args)..., result);
    if (XAML_FAILED(hr)) *pres = hr;
    return result;
}

template <typename Return, typename... Args, std::int32_t... Indicies>
xaml_result XAML_CALL __xaml_delegate_noexcept_impl_invoke_impl(Return* presult, std::function<xaml_result(Args..., Return*)> const& func, xaml_vector_view* args, std::integer_sequence<std::int32_t, Indicies...>) noexcept
{
    xaml_result hr = XAML_S_OK;
    XAML_RETURN_IF_FAILED(func(
        __xaml_delegate_noexcept_invoke<Args>(
            [args](int32_t i, auto& arg) -> xaml_result {
                xaml_ptr<xaml_object> item;
                XAML_RETURN_IF_FAILED(args->get_at(i, &item));
                return xaml_unbox_value<Args>(item.get(), arg);
            },
            &hr, Indicies)...,
        presult));
    return hr;
}

template <typename Return, typename... Args>
xaml_result XAML_CALL __xaml_delegate_noexcept_impl_invoke(Return* presult, std::function<xaml_result(Args..., Return*)> const& func, xaml_vector_view* args)
{
    return __xaml_delegate_noexcept_impl_invoke_impl<Return, Args...>(func, args, std::make_integer_sequence<std::int32_t, sizeof...(Args)>{});
}

template <typename... Args, std::int32_t... Indicies>
xaml_result XAML_CALL __xaml_delegate_noexcept_impl_invoke_void_impl(std::function<xaml_result(Args...)> const& func, xaml_vector_view* args, std::integer_sequence<std::int32_t, Indicies...>) noexcept
{
    xaml_result hr = XAML_S_OK;
    XAML_RETURN_IF_FAILED(func(__xaml_delegate_noexcept_invoke<Args>(
        [args](int32_t i, auto& arg) -> xaml_result {
            xaml_ptr<xaml_object> item;
            XAML_RETURN_IF_FAILED(args->get_at(i, &item));
            return xaml_unbox_value<Args>(item.get(), arg);
        },
        &hr, Indicies)...));
    return hr;
}

template <typename... Args>
xaml_result XAML_CALL __xaml_delegate_noexcept_impl_invoke_void(std::function<xaml_result(Args...)> const& func, xaml_vector_view* args)
{
    return __xaml_delegate_noexcept_impl_invoke_void_impl<Args...>(func, args, std::make_integer_sequence<std::int32_t, sizeof...(Args)>{});
}

template <typename Return, typename... Args, typename F>
inline xaml_result XAML_CALL xaml_delegate_new_noexcept(F&& func, xaml_delegate** ptr) noexcept
{
    return xaml_delegate_new(
        std::function<xaml_result(xaml_vector_view*, xaml_object**)>{
            [func = std::function<xaml_result(Args...)>(std::forward<F>(func))](xaml_vector_view* args, xaml_object** ptr) -> xaml_result {
                std::int32_t size;
                XAML_RETURN_IF_FAILED(args->get_size(&size));
                if (size < sizeof...(Args)) return XAML_E_INVALIDARG;
                if constexpr (std::is_same_v<Return, void>)
                {
                    XAML_RETURN_IF_FAILED(__xaml_delegate_noexcept_impl_invoke_void(func, args));
                    *ptr = nullptr;
                    return XAML_S_OK;
                }
                else
                {
                    Return result;
                    XAML_RETURN_IF_FAILED(__xaml_delegate_noexcept_impl_invoke(&result, func, args));
                    return xaml_box_value(result, ptr);
                }
            } },
        ptr);
}

template <typename... Args>
xaml_result XAML_CALL xaml_delegate_pack_args(xaml_vector_view** ptr, Args&&... args) noexcept
{
    try
    {
        xaml_ptr<xaml_vector> res;
        XAML_RETURN_IF_FAILED(xaml_vector_new({ xaml_box_value(std::forward<Args>(args))... }, &res));
        return res->query(ptr);
    }
    XAML_CATCH_RETURN()
}

template <typename T, typename Return, typename... Args>
constexpr decltype(auto) xaml_mem_fn(Return (XAML_CALL T::*f)(Args...), T* obj) noexcept
{
    return [=](Args... args) -> Return { return (obj->*f)(std::forward<Args>(args)...); };
}
#endif // __cplusplus

#endif // !XAML_DELEGATE_H
