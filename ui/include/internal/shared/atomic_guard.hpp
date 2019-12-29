#ifndef XAML_UI_INTERNAL_SHARED_ATOMIC_GUARD_HPP
#define XAML_UI_INTERNAL_SHARED_ATOMIC_GUARD_HPP

#include <atomic>

namespace xaml
{
    template <typename T>
    struct atomic_guard
    {
    private:
        std::atomic<T>& m_atomic_ref;
        T m_old_value;

    public:
        atomic_guard(std::atomic<T>& ref) : m_atomic_ref(ref) {}

        T exchange(T value) { return m_old_value = m_atomic_ref.exchange(value); }

        ~atomic_guard() { m_atomic_ref = m_old_value; }
    };
} // namespace xaml

#endif // !XAML_UI_INTERNAL_SHARED_ATOMIC_GUARD_HPP
