#ifndef XAML_DELEGATE_H
#define XAML_DELEGATE_H

#ifdef __cplusplus
    #include <functional>
    #include <xaml/box.h>
    #include <xaml/ptr.hpp>
#endif // __cplusplus

#include <xaml/object.h>
#include <xaml/vector.h>

XAML_TYPE_BASE(xaml_delegate_2, { 0x3a1a793a, 0x8a83, 0x4d40, { 0x89, 0x62, 0x03, 0x9e, 0x00, 0x0c, 0x0f, 0xc3 } })

#define XAML_DELEGATE_2_VTBL(type, TA1N, TA1I, TA2N, TA2I) \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));             \
    XAML_METHOD(invoke, type, TA1I, TA2I)

#ifdef __cplusplus
template <typename... Args>
struct xaml_delegate : xaml_object
{
    virtual xaml_result XAML_CALL invoke(xaml_interface_t<Args>...) noexcept = 0;
};

template <typename... Args>
struct xaml_base<xaml_delegate<Args...>>
{
    using type = xaml_object;
};

template <typename... Args>
struct xaml_type_guid<xaml_delegate<Args...>>
{
    static constexpr xaml_guid value = xaml_guid_xaml_delegate_2;
};

    #define XAML_DELEGATE_2_NAME(a1type, a2type) xaml_delegate<a1type, a2type>

    #define __XAML_DELEGATE_2_TYPE(a1type, a2type) typedef xaml_delegate<a1type, a2type> xaml_delegate_2__##a1type##__##a2type;
#else
    #define XAML_DELEGATE_2_NAME(a1type, a2type) xaml_delegate_2__##a1type##__##a2type

    #define __XAML_DELEGATE_2_TYPE(a1type_name, a1type_interface, a2type_name, a2type_interface) \
        XAML_DECL_INTERFACE_T_(xaml_delegate_2, a1type_name##__##a2type_name, XAML_DELEGATE_2_VTBL, a1type_name, a1type_interface, a2type_name, a2type_interface)
#endif // __cplusplus
#define XAML_DELEGATE_2_TYPE(t1, t2) __XAML_DELEGATE_2_TYPE(t1, t2)

#ifdef __cplusplus
template <typename... Args>
struct __xaml_delegate_implement : xaml_implement<__xaml_delegate_implement<Args...>, xaml_delegate<Args...>>
{
    using func_type = std::function<xaml_result(xaml_interface_t<Args>...)>;

    func_type m_func;

    __xaml_delegate_implement(func_type&& func) noexcept : m_func(std::move(func)) {}

    xaml_result XAML_CALL invoke(xaml_interface_t<Args>... args) noexcept override
    {
        return m_func(args...);
    }
};

template <typename... Args, typename F>
xaml_result XAML_CALL xaml_delegate_new(F&& func, xaml_delegate<Args...>** ptr) noexcept
{
    return xaml_object_new_catch<__xaml_delegate_implement<Args...>>(ptr, std::move(func));
}

template <typename T, typename Return, typename... Args>
constexpr decltype(auto) xaml_mem_fn(Return (XAML_CALL T::*f)(Args...), T* obj) noexcept
{
    return [=](Args... args) -> Return { return (obj->*f)(std::forward<Args>(args)...); };
}

template <typename T, typename Return, typename... Args>
constexpr decltype(auto) xaml_mem_fn(Return (XAML_CALL T::*f)(Args...)) noexcept
{
    return [=](T* obj, Args... args) -> Return { return (obj->*f)(std::forward<Args>(args)...); };
}
#endif // __cplusplus

#endif // !XAML_DELEGATE_H
