#ifndef XAML_DELEGATE_HPP
#define XAML_DELEGATE_HPP

#include <functional>
#include <xaml/delegate.h>
#include <xaml/object.hpp>
#include <xaml/xaml_ptr.hpp>

template <typename T>
struct __xaml_delegate_impl;

template <typename Return, typename... Args>
struct __xaml_delegate_impl<Return(Args...)> : xaml_implement<__xaml_delegate_impl<Return(Args...)>, xaml_delegate, xaml_delegate, xaml_object>
{
private:
    std::function<Return(Args...)> m_func;

public:
    __xaml_delegate_impl(std::function<Return(Args...)>&& func) noexcept : m_func(std::move(func)) {}

    xaml_result XAML_CALL invoke(xaml_vector_view* args, xaml_object** ptr) const noexcept override
    {
        try
        {
            // TODO: invoke with unbox_value and return with box_value
            return XAML_S_OK;
        }
        catch (...)
        {
            return XAML_E_FAIL;
        }
    }
};

template <typename T>
inline xaml_ptr<__xaml_delegate_impl<T>> xaml_delegate_new(std::function<T>&& func)
{
    return new __xaml_delegate_impl<T>(std::move(func));
}

#endif // !XAML_DELEGATE_HPP
