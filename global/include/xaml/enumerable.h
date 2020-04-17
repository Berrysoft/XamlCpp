#ifndef XAML_ENUMERABLE_H
#define XAML_ENUMERABLE_H

#ifdef __cplusplus
#include <xaml/xaml_ptr.hpp>
#endif // __cplusplus

#include <xaml/object.h>

XAML_CLASS(xaml_enumerator, { 0x4f706e46, 0x5b78, 0x4504, { 0xbc, 0x4c, 0x4a, 0x0c, 0x7d, 0x34, 0x9e, 0x11 } })

#ifdef __cplusplus
struct xaml_enumerator : xaml_object
{
    virtual xaml_result XAML_CALL move_next(bool*) noexcept = 0;
    virtual xaml_result XAML_CALL get_current(xaml_object**) noexcept = 0;
};
#else
#define XAML_ENUMERATOR_VTBL(type)                          \
    xaml_result(XAML_CALL* move_next)(type* const, _Bool*); \
    xaml_result(XAML_CALL* get_current)(type* const, xaml_object**);

struct xaml_enumerator
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_enumerator)
        XAML_ENUMERATOR_VTBL(xaml_enumerator)
    } const* vtbl;
};
#endif // __cplusplus

XAML_CLASS(xaml_enumerable, { 0x7d0d584f, 0x9d47, 0x4375, { 0x8a, 0x4b, 0xab, 0x09, 0x0f, 0xc2, 0xb0, 0x95 } })

#ifdef __cplusplus
struct xaml_enumerable : xaml_object
{
    virtual xaml_result XAML_CALL get_enumerator(xaml_enumerator**) noexcept = 0;
};
#else
#define XAML_ENUMERABLE_VTBL(type) \
    xaml_result(XAML_CALL* get_enumerator)(type* const, xaml_enumerator**);

struct xaml_enumerable
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_enumerable)
        XAML_ENUMERABLE_VTBL(xaml_enumerable)
    } const* vtbl;
};
#endif // __cplusplus

#ifdef __cplusplus
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
inline __xaml_enumerator_iterator end(xaml_ptr<T> const&) noexcept
{
    return {};
}
#endif // __cplusplus

#endif // !XAML_ENUMERABLE_H
