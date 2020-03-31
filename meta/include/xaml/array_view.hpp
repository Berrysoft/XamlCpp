#ifndef XAML_ARRAY_VIEW_HPP
#define XAML_ARRAY_VIEW_HPP

#include <algorithm>
#include <array>
#include <iterator>
#include <vector>

namespace xaml
{
    template <typename T>
    class array_view
    {
    public:
        using value_type = T;
        using pointer = value_type const*;
        using const_pointer = pointer;
        using reference = value_type const&;
        using const_reference = reference;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<pointer>;
        using const_reverse_iterator = std::reverse_iterator<const_pointer>;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

    private:
        pointer m_start{ nullptr };
        size_type m_count{ 0 };

        [[noreturn]] static void throw_out_of_range()
        {
            throw std::out_of_range{ "invalid array_view position" };
        }

    public:
        constexpr array_view() noexcept {}
        constexpr array_view(std::vector<T> const& list) noexcept : m_start(list.data()), m_count(list.size())
        {
        }
        template <size_type N>
        constexpr array_view(std::array<T, N> const& arr) noexcept : m_start(arr.data()), m_count(arr.size())
        {
        }
        template <size_type N>
        constexpr array_view(value_type const (&arr)[N]) noexcept : m_start(arr), m_count(N)
        {
        }
        constexpr array_view(const_pointer p, size_type size) noexcept : m_start(p), m_count(size)
        {
        }

        constexpr array_view& operator=(std::vector<T> const& list) noexcept
        {
            m_start = list.data();
            m_count = list.size();
            return *this;
        }
        template <size_type N>
        constexpr array_view& operator=(std::array<T, N> const& arr) noexcept
        {
            m_start = arr.data();
            m_count = arr.size();
            return *this;
        }
        template <size_type N>
        constexpr array_view& operator=(value_type (&arr)[N]) noexcept
        {
            m_start = arr;
            m_count = N;
            return *this;
        }

        constexpr const_reference operator[](size_type index) const noexcept { return m_start[index]; }
        constexpr const_reference at(size_type pos) const
        {
            if (m_count <= pos) throw_out_of_range();
            return m_start[pos];
        }

        constexpr size_type size() const noexcept { return m_count; }
        [[nodiscard]] constexpr bool empty() const noexcept { return !m_count; }

        constexpr const_pointer data() const noexcept { return m_start; }

        constexpr const_iterator cbegin() const noexcept { return m_start; }
        constexpr const_iterator cend() const noexcept { return m_start + m_count; }
        constexpr iterator begin() const noexcept { return cbegin(); }
        constexpr iterator end() const noexcept { return cend(); }

        constexpr const_reverse_iterator crbegin() const noexcept { return std::reverse_iterator(cend()); }
        constexpr const_reverse_iterator crend() const noexcept { return std::reverse_iterator(cbegin()); }
        constexpr reverse_iterator rbegin() const noexcept { return crbegin(); }
        constexpr reverse_iterator rend() const noexcept { return crend(); }

        constexpr const_reference front() const { return *m_start; }
        constexpr const_reference back() const { return m_start[m_count - 1]; }

        static constexpr size_type npos = size_type(-1);

        constexpr array_view subarr(size_type pos = 0, size_type count = npos) const
        {
            if (pos > m_count) throw_out_of_range();
            return array_view<T>(m_start + pos, (std::min)(count, m_count - pos));
        }

        operator std::vector<T>() const { return std::vector<T>(m_start, m_start + m_count); }
    };
} // namespace xaml

#endif // !XAML_ARRAY_VIEW_HPP
