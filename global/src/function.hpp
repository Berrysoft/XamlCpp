#ifndef XAML_GLOBAL_INTERNAL_FUNCTION_HPP
#define XAML_GLOBAL_INTERNAL_FUNCTION_HPP

#include <function2/function2.hpp>
#include <functional>

template <template <typename> typename Wrapper, typename Func>
struct xaml_function_wrap_helper;

template <template <typename> typename Wrapper, typename Return, typename... Args>
struct xaml_function_wrap_helper<Wrapper, Return(Args...)>
{
    Wrapper<Return(Args...) noexcept> operator()(std::function<Return(Args...)>&& func) const
    {
        return [func = std::move(func)](Args... args) noexcept -> Return { return func(std::forward<Args>(args)...); };
    }
};

template <typename Func>
inline auto xaml_function_wrap(std::function<Func> func)
{
    return xaml_function_wrap_helper<fu2::function, Func>{}(std::move(func));
}

template <typename Func>
inline auto xaml_function_wrap_unique(std::function<Func> func)
{
    return xaml_function_wrap_helper<fu2::unique_function, Func>{}(std::move(func));
}

#endif // !XAML_GLOBAL_INTERNAL_FUNCTION_HPP
