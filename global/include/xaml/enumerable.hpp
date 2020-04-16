#ifndef XAML_ENUMERABLE_HPP
#define XAML_ENUMERABLE_HPP

#include <xaml/enumerable.h>
#include <xaml/result.hpp>
#include <xaml/xaml_ptr.hpp>

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

#endif // !XAML_ENUMERABLE_HPP
