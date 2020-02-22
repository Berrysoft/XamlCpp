#ifndef XAML_UI_INTERNAL_SHARED_ATOMIC_GUARD_HPP
#define XAML_UI_INTERNAL_SHARED_ATOMIC_GUARD_HPP

#include <atomic>
#include <optional>

namespace xaml
{
    template <typename T>
    struct atomic_guard
    {
    private:
        std::atomic<T>& m_atomic_ref;
        std::optional<T> m_old_value;

    public:
        atomic_guard(std::atomic<T>& ref) : m_atomic_ref(ref), m_old_value(std::nullopt) {}

        T exchange(T value) { return *(m_old_value = m_atomic_ref.exchange(value)); }

        ~atomic_guard()
        {
            if (m_old_value) m_atomic_ref = *m_old_value;
        }
    };
} // namespace xaml

#endif // !XAML_UI_INTERNAL_SHARED_ATOMIC_GUARD_HPP
