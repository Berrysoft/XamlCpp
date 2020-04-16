#ifndef XAML_CALLBACK_HPP
#define XAML_CALLBACK_HPP

#include <functional>
#include <xaml/box.hpp>
#include <xaml/callback.h>
#include <xaml/object.hpp>
#include <xaml/result.hpp>

template <typename T>
struct __xaml_callback_impl;

template <typename T1, typename T2>
struct __xaml_callback_impl<void(T1, T2)> : xaml_implement<__xaml_callback_impl<void(T1, T2)>, xaml_callback, xaml_object>
{
private:
    std::function<void(T1, T2)> m_func;

public:
    __xaml_callback_impl(std::function<void(T1, T2)>&& func) noexcept : m_func(std::move(func)) {}

    xaml_result XAML_CALL invoke(xaml_object* sender, xaml_object* args) const noexcept override
    {
        try
        {
            T1 arg1;
            XAML_RETURN_IF_FAILED(unbox_value(sender, arg1));
            T2 arg2;
            XAML_RETURN_IF_FAILED(unbox_value(args, arg2));
            m_func(arg1, arg2);
            return XAML_S_OK;
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
};

template <typename T>
inline xaml_result xaml_callback_new(std::function<T>&& func, xaml_callback** ptr) noexcept
{
    if (!func) return XAML_E_INVALIDARG;
    return xaml_object_new<__xaml_callback_impl<T>>(ptr, std::move(func));
}

#endif // !XAML_CALLBACK_HPP
