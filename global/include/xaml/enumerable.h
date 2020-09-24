#ifndef XAML_ENUMERABLE_H
#define XAML_ENUMERABLE_H

#ifdef __cplusplus
    #include <xaml/ptr.hpp>
#else
    #include <stdbool.h>
#endif // __cplusplus

#include <xaml/object.h>

__XAML_TYPE_NAME_BASE(xaml_enumerator, { 0x4f706e46, 0x5b78, 0x4504, { 0xbc, 0x4c, 0x4a, 0x0c, 0x7d, 0x34, 0x9e, 0x11 } })

#define XAML_ENUMERATOR_T_VTBL(type, TN, TI)   \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(move_next, type, bool*);       \
    XAML_METHOD(get_current, type, TI*)

#ifdef __cplusplus
XAML_DECL_INTERFACE_T_(xaml_enumerator, xaml_object, XAML_ENUMERATOR_T_VTBL)

    #define XAML_ENUMERATOR_T_NAME(type) xaml_enumerator<type>

    #define XAML_ENUMERATOR_T_TYPE(type) typedef xaml_enumerator<type> xaml_enumerator__##type##__;

    #define XAML_ENUMERATOR_T_V_TYPE(type) XAML_ENUMERATOR_T_TYPE(type)
    #define XAML_ENUMERATOR_T_O_TYPE(type) XAML_ENUMERATOR_T_TYPE(type)
#else
    #define XAML_ENUMERATOR_T_NAME(type) xaml_enumerator__##type##__

    #define XAML_ENUMERATOR_T_TYPE(type_name, type_interface) \
        XAML_DECL_INTERFACE_T_(xaml_enumerator, type_name, XAML_ENUMERATOR_T_VTBL, type_name, type_interface)

    #define XAML_ENUMERATOR_T_V_TYPE(type) XAML_ENUMERATOR_T_TYPE(type, type)
    #define XAML_ENUMERATOR_T_O_TYPE(type) XAML_ENUMERATOR_T_TYPE(type, type*)
#endif // __cplusplus

__XAML_TYPE_NAME_BASE(xaml_enumerable, { 0x7d0d584f, 0x9d47, 0x4375, { 0x8a, 0x4b, 0xab, 0x09, 0x0f, 0xc2, 0xb0, 0x95 } })

#define XAML_ENUMERABLE_T_VTBL(type, TN, TI)   \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(get_enumerator, type, XAML_ENUMERATOR_T_NAME(TN)**)

#ifdef __cplusplus
XAML_DECL_INTERFACE_T_(xaml_enumerable, xaml_object, XAML_ENUMERABLE_T_VTBL)

    #define XAML_ENUMERABLE_T_TYPE(type) typedef xaml_enumerable<type> xaml_enumerable__##type##__;

    #define XAML_ENUMERABLE_T_V_TYPE(type) XAML_ENUMERABLE_T_TYPE(type)
    #define XAML_ENUMERABLE_T_O_TYPE(type) XAML_ENUMERABLE_T_TYPE(type)
#else
    #define XAML_ENUMERABLE_T_TYPE(type_name, type_interface) \
        XAML_DECL_INTERFACE_T_(xaml_enumerable, type_name, XAML_ENUMERABLE_T_VTBL, type_name, type_interface)

    #define XAML_ENUMERABLE_T_V_TYPE(type) XAML_ENUMERABLE_T_TYPE(type, type)
    #define XAML_ENUMERABLE_T_O_TYPE(type) XAML_ENUMERABLE_T_TYPE(type, type*)
#endif // __cplusplus

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
struct xaml_enumerable_wrapper
{
    xaml_ptr<xaml_enumerable<T>> m_enumerable;

    xaml_enumerable_wrapper(xaml_ptr<xaml_enumerable<T>> const& enumerable) : m_enumerable(enumerable)
    {
    }

    __xaml_enumerator_iterator<T> begin()
    {
        xaml_ptr<xaml_enumerator<T>> e;
        XAML_THROW_IF_FAILED(m_enumerable->get_enumerator(&e));
        bool ok;
        XAML_THROW_IF_FAILED(e->move_next(&ok));
        if (ok)
            return { std::move(e) };
        else
            return {};
    }

    __xaml_enumerator_iterator<T> end() noexcept
    {
        return {};
    }
};
#endif // __cplusplus

#endif // !XAML_ENUMERABLE_H
