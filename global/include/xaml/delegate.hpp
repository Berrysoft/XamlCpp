#ifndef XAML_DELEGATE_HPP
#define XAML_DELEGATE_HPP

#include <functional>
#include <xaml/delegate.h>
#include <xaml/object.hpp>
#include <xaml/result.hpp>
#include <xaml/xaml_ptr.hpp>

template <typename T>
struct __xaml_delegate_impl;

template <typename Return, typename... Args>
struct __xaml_delegate_impl<Return(Args...)> : xaml_implement<__xaml_delegate_impl<Return(Args...)>, xaml_delegate, xaml_object>
{
private:
    std::function<Return(Args...)> m_func;

public:
    __xaml_delegate_impl(std::function<Return(Args...)>&& func) noexcept : m_func(std::move(func)) {}

    xaml_result XAML_CALL invoke(xaml_vector_view* args, xaml_object** ptr) const noexcept override;
};

inline xaml_ptr<xaml_object> __xaml_delegate_impl_invoke_impl_get_at(xaml_vector_view* args, size_t index)
{
    xaml_ptr<xaml_object> item;
    XAML_THROW_IF_FAILED(args->get_at(index, &item));
    return item;
}

template <typename Return, typename... Args, size_t... Indicies>
Return __xaml_delegate_impl_invoke_impl(std::function<Return(Args...)> const& func, xaml_vector_view* args, std::index_sequence<Indicies...>)
{
    return func(unbox_value<Args>(__xaml_delegate_impl_invoke_impl_get_at(args, Indicies))...);
}

template <typename Return, typename... Args>
Return __xaml_delegate_impl_invoke(std::function<Return(Args...)> const& func, xaml_vector_view* args)
{
    return __xaml_delegate_impl_invoke_impl(func, args, std::make_size_sequence<sizeof...(Args)>);
}

template <typename Return, typename... Args>
xaml_result __xaml_delegate_impl<Return(Args...)>::invoke(xaml_vector_view* args, xaml_object** ptr) const noexcept
{
    size_t size;
    RETURN_IF_FAILED(args->get_size(&size));
    if (size != sizeof...(Args)) return XAML_E_INVALIDARG;
    try
    {
        Return res = __xaml_delegate_impl_invoke(m_func, args);
        return box_value(res, ptr);
    }
    catch (xaml_result_error const& e)
    {
        return e.get_result();
    }
    catch (...)
    {
        return XAML_E_FAIL;
    }
}

template <typename T>
inline xaml_result xaml_delegate_new(std::function<T>&& func, xaml_delegate** ptr) noexcept
{
    if (!func) return XAML_E_INVALIDARG;
    return xaml_object_new<__xaml_delegate_impl<T>>(ptr, std::move(func));
}

#endif // !XAML_DELEGATE_HPP
