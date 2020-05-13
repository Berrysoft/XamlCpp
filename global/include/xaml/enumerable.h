#ifndef XAML_ENUMERABLE_H
#define XAML_ENUMERABLE_H

#ifdef __cplusplus
#include <xaml/ptr.hpp>
#else
#include <stdbool.h>
#endif // __cplusplus

#include <xaml/object.h>

XAML_CLASS(xaml_enumerator, { 0x4f706e46, 0x5b78, 0x4504, { 0xbc, 0x4c, 0x4a, 0x0c, 0x7d, 0x34, 0x9e, 0x11 } })

#define XAML_ENUMERATOR_VTBL(type)             \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(move_next, type, bool*);       \
    XAML_METHOD(get_current, type, xaml_object**)

XAML_DECL_INTERFACE_(xaml_enumerator, xaml_object)
{
    XAML_DECL_VTBL(xaml_enumerator, XAML_ENUMERATOR_VTBL);
};

XAML_CLASS(xaml_enumerable, { 0x7d0d584f, 0x9d47, 0x4375, { 0x8a, 0x4b, 0xab, 0x09, 0x0f, 0xc2, 0xb0, 0x95 } })

#define XAML_ENUMERABLE_VTBL(type)             \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(get_enumerator, type, xaml_enumerator**)

XAML_DECL_INTERFACE_(xaml_enumerable, xaml_object)
{
    XAML_DECL_VTBL(xaml_enumerable, XAML_ENUMERABLE_VTBL);
};

#ifdef __cplusplus
#define XAML_FOREACH_START(item, enumerable)                     \
    do                                                           \
    {                                                            \
        xaml_ptr<xaml_enumerator> __e;                           \
        XAML_RETURN_IF_FAILED(enumerable->get_enumerator(&__e)); \
        while (true)                                             \
        {                                                        \
            bool __moved;                                        \
            XAML_RETURN_IF_FAILED(__e->move_next(&__moved));     \
            if (!__moved) break;                                 \
            xaml_ptr<xaml_object> item;                          \
            XAML_RETURN_IF_FAILED(__e->get_current(&item))

#define XAML_FOREACH_END() \
    }                      \
    }                      \
    while (0)

struct __xaml_enumerator_iterator
{
private:
    xaml_ptr<xaml_enumerator> m_enumerator{ nullptr };

public:
    __xaml_enumerator_iterator() noexcept {}
    __xaml_enumerator_iterator(xaml_ptr<xaml_enumerator>&& e) noexcept : m_enumerator(std::move(e)) {}

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

    xaml_ptr<xaml_object> operator*() const
    {
        if (!m_enumerator) return nullptr;
        xaml_ptr<xaml_object> res;
        XAML_THROW_IF_FAILED(m_enumerator->get_current(&res));
        return res;
    }

    bool operator==(__xaml_enumerator_iterator const& rhs) const
    {
        return m_enumerator == nullptr && rhs.m_enumerator == nullptr;
    }

    bool operator!=(__xaml_enumerator_iterator const& rhs) const
    {
        return !operator==(rhs);
    }
};

template <typename T, typename = std::enable_if_t<std::is_base_of_v<xaml_enumerable, T>>>
inline __xaml_enumerator_iterator begin(xaml_ptr<T> const& enumerable)
{
    xaml_ptr<xaml_enumerator> e;
    XAML_THROW_IF_FAILED(enumerable->get_enumerator(&e));
    bool ok;
    XAML_THROW_IF_FAILED(e->move_next(&ok));
    if (ok)
        return { std::move(e) };
    else
        return {};
}

template <typename T, typename = std::enable_if_t<std::is_base_of_v<xaml_enumerable, T>>>
inline __xaml_enumerator_iterator begin(T* enumerable)
{
    return begin(xaml_ptr<T>(enumerable));
}

template <typename T, typename = std::enable_if_t<std::is_base_of_v<xaml_enumerable, T>>>
inline __xaml_enumerator_iterator end(xaml_ptr<T> const&) noexcept
{
    return {};
}

template <typename T, typename = std::enable_if_t<std::is_base_of_v<xaml_enumerable, T>>>
inline __xaml_enumerator_iterator end(T*) noexcept
{
    return {};
}
#endif // __cplusplus

#endif // !XAML_ENUMERABLE_H
