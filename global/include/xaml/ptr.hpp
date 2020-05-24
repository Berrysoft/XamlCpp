#ifndef XAML_PTR_HPP
#define XAML_PTR_HPP

#include <functional>
#include <utility>
#include <xaml/result.h>

template <typename T>
struct xaml_ptr
{
private:
    T* m_ptr{};

    constexpr void try_release() noexcept
    {
        if (m_ptr) m_ptr->release();
    }

    constexpr void try_add() noexcept
    {
        if (m_ptr) m_ptr->add_ref();
    }

    template <typename D>
    friend struct xaml_ptr;

public:
    constexpr xaml_ptr() noexcept : m_ptr{ nullptr } {}
    constexpr xaml_ptr(std::nullptr_t) noexcept : m_ptr{ nullptr } {}
    constexpr xaml_ptr(T* ptr) noexcept : m_ptr{ ptr } { try_add(); }
    constexpr xaml_ptr(xaml_ptr const& p) noexcept : m_ptr{ p.m_ptr } { try_add(); }
    template <typename D, typename = std::enable_if_t<std::is_base_of_v<T, D>>>
    constexpr xaml_ptr(D* ptr) noexcept : m_ptr{ ptr }
    {
        try_add();
    }
    template <typename D, typename = std::enable_if_t<std::is_base_of_v<T, D>>>
    constexpr xaml_ptr(xaml_ptr<D> const& p) noexcept : m_ptr{ p.m_ptr }
    {
        try_add();
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
        try_add();
        return *this;
    }

    template <typename D, typename = std::enable_if_t<std::is_base_of_v<T, D>>>
    constexpr xaml_ptr& operator=(D* ptr) noexcept
    {
        try_release();
        m_ptr = ptr;
        try_add();
        return *this;
    }

    constexpr xaml_ptr& operator=(xaml_ptr const& p) noexcept
    {
        try_release();
        m_ptr = p.m_ptr;
        try_add();
        return *this;
    }

    template <typename D, typename = std::enable_if_t<std::is_base_of_v<T, D>>>
    constexpr xaml_ptr& operator=(xaml_ptr<D> const& p) noexcept
    {
        try_release();
        m_ptr = p.m_ptr;
        try_add();
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
    constexpr T* const* operator&() const noexcept { return &m_ptr; }
    constexpr T* operator->() const noexcept { return m_ptr; }
    constexpr T& operator*() const noexcept { return *m_ptr; }

    constexpr operator T*() const noexcept { return m_ptr; }

    constexpr T* get() const noexcept { return m_ptr; }
    [[nodiscard]] constexpr T* detach() const noexcept
    {
        T* res = m_ptr;
        m_ptr = nullptr;
        return res;
    }
    constexpr void reset() noexcept
    {
        try_release();
        m_ptr = nullptr;
    }
    [[nodiscard]] constexpr T** put() noexcept
    {
        reset();
        return &m_ptr;
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

    template <typename D>
    constexpr xaml_result query(D** ptr) const noexcept
    {
        if (m_ptr)
        {
            return m_ptr->query(ptr);
        }
        else
        {
            *ptr = nullptr;
            return XAML_S_OK;
        }
    }

    constexpr operator bool() const noexcept { return m_ptr; }

    void swap(xaml_ptr& other) noexcept
    {
        std::swap(m_ptr, other.m_ptr);
    }

    template <typename D>
    constexpr bool operator==(xaml_ptr<D> const& rhs) const noexcept
    {
        return m_ptr == rhs.m_ptr;
    }

    template <typename D>
    constexpr bool operator==(D* rhs) const noexcept
    {
        return m_ptr == rhs;
    }

    friend constexpr bool operator==(xaml_ptr<T> const& lhs, std::nullptr_t) noexcept { return lhs.m_ptr == nullptr; }
    friend constexpr bool operator==(std::nullptr_t, xaml_ptr<T> const& rhs) noexcept { return rhs == nullptr; }
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

template <typename T>
inline std::size_t hash_value(xaml_ptr<T> const& ptr) noexcept
{
    return std::hash<xaml_ptr<T>>{}(ptr);
}

#endif // !XAML_PTR_HPP
