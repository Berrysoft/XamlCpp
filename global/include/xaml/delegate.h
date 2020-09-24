#ifndef XAML_DELEGATE_H
#define XAML_DELEGATE_H

#ifdef __cplusplus
    #include <functional>
    #include <xaml/box.h>
    #include <xaml/ptr.hpp>
#endif // __cplusplus

#include <xaml/object.h>
#include <xaml/vector.h>

__XAML_TYPE_NAME_BASE(xaml_delegate, { 0x3a1a793a, 0x8a83, 0x4d40, { 0x89, 0x62, 0x03, 0x9e, 0x00, 0x0c, 0x0f, 0xc3 } })

#define XAML_DELEGATE_T_VTBL(type, TN, TI)     \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(invoke, type, TI)

#define XAML_DELEGATE_T_T_VTBL(type, TA1N, TA1I, TA2N, TA2I) \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));               \
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
    static constexpr xaml_guid value = xaml_guid_xaml_delegate;
};

    #define XAML_DELEGATE_T_TYPE(type) typedef xaml_delegate<type> xaml_delegate__##type##__;

    #define XAML_DELEGATE_T_V_TYPE(type) XAML_DELEGATE_T_TYPE(type)
    #define XAML_DELEGATE_T_O_TYPE(type) XAML_DELEGATE_T_TYPE(type)

    #define XAML_DELEGATE_T_T_TYPE(a1type, a2type) typedef xaml_delegate<rtype, a1type, a2type> xaml_delegate__##a1type##____##a2type##__;

    #define XAML_DELEGATE_T_O_T_O_TYPE(a1type, a2type) XAML_DELEGATE_T_T_TYPE(a1type, a2type)
#else
    #define XAML_DELEGATE_T_TYPE(type_name, type_interface) \
        XAML_DECL_INTERFACE_T_(xaml_delegate, XAML_DELEGATE_T_VTBL, type_name, type_interface)

    #define XAML_DELEGATE_T_V_TYPE(type) XAML_DELEGATE_T_TYPE(type, type)
    #define XAML_DELEGATE_T_O_TYPE(type) XAML_DELEGATE_T_type(type, type*)

    #define XAML_DELEGATE_T_T_TYPE(a1type_name, a1type_interface, a2type_name, a2type_interface) \
        XAML_DECL_INTERFACE_T_(xaml_delegate, a1type_name##____##a2type_name, XAML_DELEGATE_T_T_VTBL, a1type_name, a1type_interface, a2type_name, a2type_interface)

    #define XAML_DELEGATE_T_O_T_O_TYPE(a1type, a2type) XAML_DELEGATE_T_T_T_TYPE(a1type, a1type*, a2type, a2type*)
#endif // __cplusplus

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

template <typename... Args>
xaml_result XAML_CALL xaml_delegate_new(std::function<xaml_result(xaml_interface_t<Args>...)>&& func, xaml_delegate<Args...>** ptr) noexcept
{
    return xaml_object_new<__xaml_delegate_implement<Args...>>(ptr, std::move(func));
}

template <typename T, typename Return, typename... Args>
constexpr decltype(auto) xaml_mem_fn(Return (XAML_CALL T::*f)(Args...), T* obj) noexcept
{
    return [=](Args... args) -> Return { return (obj->*f)(std::forward<Args>(args)...); };
}
#endif // __cplusplus

#endif // !XAML_DELEGATE_H
