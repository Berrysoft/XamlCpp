#ifndef XAML_PTR_HPP
#define XAML_PTR_HPP

#include <functional>
#include <utility>
#include <xaml/result.h>

template <typename T>
class xaml_ptr
{
private:
    T* m_ptr;

    constexpr void try_release() noexcept
    {
        if (m_ptr) m_ptr->release();
    }

public:
    constexpr xaml_ptr() noexcept : m_ptr{ nullptr } {}
    constexpr xaml_ptr(std::nullptr_t) noexcept : m_ptr{ nullptr } {}
    constexpr xaml_ptr(T* ptr) noexcept : m_ptr{ ptr }
    {
        if (m_ptr) m_ptr->add_ref();
    }
    constexpr xaml_ptr(xaml_ptr const& p) noexcept : m_ptr{ p.m_ptr }
    {
        if (m_ptr) m_ptr->add_ref();
    }
    template <typename D, typename = std::enable_if_t<std::is_base_of_v<T, D>>>
    constexpr xaml_ptr(xaml_ptr<D> const& p) noexcept : m_ptr{ p.m_ptr }
    {
        if (m_ptr) m_ptr->add_ref();
    }
    constexpr xaml_ptr(xaml_ptr&& p) noexcept : m_ptr{ p.m_ptr } { p.m_ptr = nullptr; }

    ~xaml_ptr() { try_release(); }

    constexpr xaml_ptr& operator=(std::nullptr_t) noexcept
    {
        try_release();
        m_ptr = nullptr;
        return *this;
    }

    constexpr xaml_ptr& operator=(T* ptr) noexcept
    {
        try_release();
        m_ptr = ptr;
        if (m_ptr) m_ptr->add_ref();
        return *this;
    }

    constexpr xaml_ptr& operator=(xaml_ptr const& p) noexcept
    {
        try_release();
        m_ptr = p.m_ptr;
        if (m_ptr) m_ptr->add_ref();
        return *this;
    }

    template <typename D, typename = std::enable_if_t<std::is_base_of_v<T, D>>>
    constexpr xaml_ptr& operator=(xaml_ptr<D> const& p) noexcept
    {
        try_release();
        m_ptr = p.m_ptr;
        if (m_ptr) m_ptr->add_ref();
        return *this;
    }

    constexpr xaml_ptr& operator=(xaml_ptr&& p) noexcept
    {
        try_release();
        m_ptr = p.m_ptr;
        p.m_ptr = nullptr;
        return *this;
    }

    constexpr T** operator&() noexcept { return &m_ptr; }
    constexpr T* operator->() const noexcept { return m_ptr; }
    constexpr T& operator*() const noexcept { return *m_ptr; }

    constexpr T* get() const noexcept { return m_ptr; }
    constexpr T* release() const noexcept
    {
        T* res = m_ptr;
        m_ptr = nullptr;
        return res;
    }

    template <typename D>
    constexpr xaml_ptr<D> query() const noexcept
    {
        if (m_ptr)
        {
            xaml_ptr<D> res;
            if (XAML_SUCCEEDED(m_ptr->query(&res))) return res;
        }
        return nullptr;
    }

    constexpr operator bool() const noexcept { return m_ptr; }

    template <typename D>
    constexpr bool operator==(xaml_ptr<D> const& rhs) const noexcept
    {
        return m_ptr == rhs.m_ptr;
    }
    template <typename D>
    constexpr bool operator!=(xaml_ptr<D> const& rhs) const noexcept
    {
        return !operator==(rhs);
    }

    friend constexpr bool operator==(xaml_ptr<T> const& lhs, std::nullptr_t) noexcept { return lhs.m_ptr == nullptr; }
    friend constexpr bool operator==(std::nullptr_t, xaml_ptr<T> const& rhs) noexcept { return rhs == nullptr; }
    friend constexpr bool operator!=(xaml_ptr<T> const& lhs, std::nullptr_t) noexcept { return !(lhs == nullptr); }
    friend constexpr bool operator!=(std::nullptr_t, xaml_ptr<T> const& rhs) noexcept { return !(rhs == nullptr); }
};

namespace std
{
    template <typename T>
    struct hash<xaml_ptr<T>>
    {
        size_t operator()(xaml_ptr<T> const& ptr) const noexcept
        {
            return hash<intptr_t>{}((intptr_t)ptr.get());
        }
    };
} // namespace std

#endif // !XAML_PTR_HPP
