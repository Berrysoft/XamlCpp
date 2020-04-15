#ifndef XAML_PTR_HPP
#define XAML_PTR_HPP

#include <utility>
#include <xaml/object.h>

template <typename T>
class xaml_ptr
{
private:
    T* m_ptr;

    void try_release()
    {
        if (m_ptr) m_ptr->release();
    }

    void acquire(T* ptr)
    {
        m_ptr = ptr;
        if (m_ptr) m_ptr->add_ref();
    }

public:
    constexpr xaml_ptr() noexcept : m_ptr{ nullptr } {}
    constexpr xaml_ptr(std::nullptr_t) noexcept : m_ptr{ nullptr } {}
    xaml_ptr(T* ptr) noexcept : m_ptr{ ptr } { m_ptr->add_ref(); }
    xaml_ptr(xaml_ptr const& p) noexcept : m_ptr{ p.m_ptr } { m_ptr->add_ref(); }
    constexpr xaml_ptr(xaml_ptr&& p) noexcept : m_ptr{ p.m_ptr } { p.m_ptr = nullptr; }

    xaml_ptr& operator=(std::nullptr_t) noexcept
    {
        try_release();
        m_ptr = nullptr;
    }

    xaml_ptr& operator=(T* ptr) noexcept
    {
        try_release();
        acquire(ptr);
    }

    xaml_ptr& operator=(xaml_ptr const& p) noexcept
    {
        try_release();
        acquire(p.m_ptr);
    }

    xaml_ptr& operator=(xaml_ptr&& p) noexcept
    {
        try_release();
        m_ptr = p.m_ptr;
        p.m_ptr = nullptr;
    }

    T** operator&() noexcept { return &m_ptr; }
};

#endif // !XAML_PTR_HPP