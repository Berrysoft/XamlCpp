#ifndef XAML_UI_INTERNAL_SHARED_ATOMIC_GUARD_HPP
#define XAML_UI_INTERNAL_SHARED_ATOMIC_GUARD_HPP

#include <atomic>

namespace xaml
{
    struct atomic_guard
    {
    private:
        std::atomic_bool* m_atomic_ref;
        bool m_old_value;

    public:
        atomic_guard(std::atomic_bool& ref) noexcept : m_atomic_ref(&ref), m_old_value(true) {}

        bool test_and_set() noexcept { return m_old_value = m_atomic_ref->exchange(true); }

        ~atomic_guard() noexcept
        {
            if (!m_old_value) m_atomic_ref->store(false);
        }
    };
} // namespace xaml

#endif // !XAML_UI_INTERNAL_SHARED_ATOMIC_GUARD_HPP
