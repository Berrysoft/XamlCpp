#ifndef XAML_ENUMERABLE_H
#define XAML_ENUMERABLE_H

#ifdef __cplusplus
    #include <type_traits>
    #include <xaml/ptr.hpp>
#endif // __cplusplus

#include <xaml/object.h>

XAML_TYPE_BASE(xaml_enumerator_1, { 0x4f706e46, 0x5b78, 0x4504, { 0xbc, 0x4c, 0x4a, 0x0c, 0x7d, 0x34, 0x9e, 0x11 } })

#define XAML_ENUMERATOR_1_VTBL(type, TN, TI)   \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(move_next, type, bool*);       \
    XAML_METHOD(get_current, type, TI*)

#ifdef __cplusplus
XAML_DECL_INTERFACE_T_(xaml_enumerator, xaml_object, XAML_ENUMERATOR_1_VTBL)

    #define XAML_ENUMERATOR_1_NAME(type) xaml_enumerator<type>

    #define __XAML_ENUMERATOR_1_TYPE(type) typedef xaml_enumerator<type> xaml_enumerator_1__##type;
#else
    #define XAML_ENUMERATOR_1_NAME(type) xaml_enumerator_1__##type

    #define __XAML_ENUMERATOR_1_TYPE(type_name, type_interface) \
        XAML_DECL_INTERFACE_T_(xaml_enumerator_1, type_name, XAML_ENUMERATOR_1_VTBL, type_name, type_interface)
#endif // __cplusplus
#define XAML_ENUMERATOR_1_TYPE(type) __XAML_ENUMERATOR_1_TYPE(type)

XAML_TYPE_BASE(xaml_enumerable_1, { 0x7d0d584f, 0x9d47, 0x4375, { 0x8a, 0x4b, 0xab, 0x09, 0x0f, 0xc2, 0xb0, 0x95 } })

#define XAML_ENUMERABLE_1_VTBL(type, TN, TI)   \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(get_enumerator, type, XAML_ENUMERATOR_1_NAME(TN)**)

#ifdef __cplusplus
XAML_DECL_INTERFACE_T_(xaml_enumerable, xaml_object, XAML_ENUMERABLE_1_VTBL)

    #define XAML_ENUMERABLE_1_NAME(type) xaml_enumerable<type>

    #define __XAML_ENUMERABLE_1_TYPE(type) typedef xaml_enumerable<type> xaml_enumerable_1__##type;
#else
    #define XAML_ENUMEABLE_1_NAME(type) xaml_enumerable_1__##type

    #define __XAML_ENUMERABLE_1_TYPE(type_name, type_interface) \
        XAML_DECL_INTERFACE_T_(xaml_enumerable_1, type_name, XAML_ENUMERABLE_1_VTBL, type_name, type_interface)
#endif // __cplusplus
#define XAML_ENUMERABLE_1_TYPE(type) __XAML_ENUMERABLE_1_TYPE(type)

#ifdef __cplusplus
    #define XAML_FOREACH_START(type, item, enumerable)               \
        do                                                           \
        {                                                            \
            xaml_ptr<xaml_enumerator<type>> __e;                     \
            XAML_RETURN_IF_FAILED(enumerable->get_enumerator(&__e)); \
            while (true)                                             \
            {                                                        \
                bool __moved;                                        \
                XAML_RETURN_IF_FAILED(__e->move_next(&__moved));     \
                if (!__moved) break;                                 \
                xaml_interface_var_t<type> item;                     \
                XAML_RETURN_IF_FAILED(__e->get_current(&item))

    #define XAML_FOREACH_END() \
        }                      \
        }                      \
        while (0)

template <typename T>
struct __xaml_enumerator_iterator
{
private:
    xaml_ptr<xaml_enumerator<T>> m_enumerator{ nullptr };

public:
    __xaml_enumerator_iterator() noexcept {}
    __xaml_enumerator_iterator(xaml_ptr<xaml_enumerator<T>>&& e) noexcept : m_enumerator(std::move(e)) {}

    __xaml_enumerator_iterator& operator++()
    {
        if (m_enumerator)
        {
            bool ok;
            XAML_THROW_IF_FAILED(m_enumerator->move_next(&ok));
            if (!ok) m_enumerator = nullptr;
        }
        return *this;
    }
    __xaml_enumerator_iterator operator++(int)
    {
        __xaml_enumerator_iterator result = *this;
        operator++();
        return result;
    }

    xaml_interface_var_t<T> operator*() const
    {
        if (!m_enumerator) return {};
        xaml_interface_var_t<T> res;
        XAML_THROW_IF_FAILED(m_enumerator->get_current(&res));
        return res;
    }

    bool operator==(__xaml_enumerator_iterator const& rhs) const
    {
        return m_enumerator == nullptr && rhs.m_enumerator == nullptr;
    }
};

template <typename T>
struct __xaml_enumerator_traits;

template <typename T>
struct __xaml_enumerator_traits<xaml_enumerator<T>>
{
    using value_type = T;
};

template <typename T>
struct __xaml_function_traits;

template <typename Return, typename T, typename... Args>
struct __xaml_function_traits<Return (XAML_CALL T::*)(Args...) noexcept>
{
    using return_type = Return;
    using class_type = T;
    using arg_types = std::tuple<Args...>;
};

template <typename T>
struct __xaml_enumerable_traits
{
    using enumerator_type = std::remove_pointer_t<std::remove_pointer_t<std::tuple_element_t<0, typename __xaml_function_traits<decltype(&T::get_enumerator)>::arg_types>>>;
    using value_type = typename __xaml_enumerator_traits<enumerator_type>::value_type;
};

template <typename T>
struct __xaml_is_enumerable
{
    static constexpr bool value = std::is_convertible_v<xaml_ptr<T>, xaml_ptr<xaml_enumerable<typename __xaml_enumerable_traits<T>::value_type>>>;
};

template <typename T>
inline constexpr bool __xaml_is_enumerable_v = __xaml_is_enumerable<T>::value;

template <typename T, typename = std::enable_if_t<__xaml_is_enumerable_v<T>>>
struct __xaml_enumerable_wrapper
{
    xaml_ptr<T> m_enumerable;

    __xaml_enumerable_wrapper(xaml_ptr<T> const& enumerable) noexcept : m_enumerable(enumerable)
    {
    }

    using value_type = typename __xaml_enumerable_traits<T>::value_type;

    __xaml_enumerator_iterator<value_type> begin()
    {
        xaml_ptr<xaml_enumerator<value_type>> e;
        XAML_THROW_IF_FAILED(m_enumerable->get_enumerator(&e));
        bool ok;
        XAML_THROW_IF_FAILED(e->move_next(&ok));
        if (ok)
            return { std::move(e) };
        else
            return {};
    }

    __xaml_enumerator_iterator<value_type> end() noexcept
    {
        return {};
    }
};

template <typename T, typename = std::enable_if_t<__xaml_is_enumerable_v<T>>>
auto begin(xaml_ptr<T> ptr)
{
    return __xaml_enumerable_wrapper<T>{ ptr }.begin();
}

template <typename T, typename = std::enable_if_t<__xaml_is_enumerable_v<T>>>
auto end(xaml_ptr<T> ptr) noexcept
{
    return __xaml_enumerable_wrapper<T>{ ptr }.end();
}
#endif // __cplusplus

#endif // !XAML_ENUMERABLE_H
