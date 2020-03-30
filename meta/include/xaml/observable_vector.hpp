#ifndef XAML_OBSERVABLE_VECTOR_HPP
#define XAML_OBSERVABLE_VECTOR_HPP

#include <cassert>
#include <vector>
#include <xaml/array_view.hpp>
#include <xaml/meta/event.hpp>
#include <xaml/meta/meta_macro.hpp>

namespace xaml
{
    enum class vector_changed_action
    {
        add,
        erase,
        replace,
        move,
        reset
    };

    template <typename T>
    struct vector_changed_args
    {
        vector_changed_action action;
        array_view<T> new_items;
        std::size_t new_index;
        array_view<T> old_items;
        std::size_t old_index;
    };

    template <typename T>
    class observable_vector
    {
    private:
        using __vector_type = std::vector<T>;

        __vector_type m_vec;

    public:
        using value_type = typename __vector_type::value_type;

        class reference
        {
        private:
            value_type* m_ptr;
            observable_vector* m_vec;

        public:
            constexpr reference(value_type& ref, observable_vector& vec) noexcept : m_ptr(&ref), m_vec(&vec) {}
            constexpr reference(value_type const& ref, observable_vector const& vec) noexcept : m_ptr(const_cast<value_type*>(&ref)), m_vec(const_cast<observable_vector*>(&vec)) {}

            reference& operator=(value_type const& ref) noexcept
            {
                auto old_item = std::move(*m_ptr);
                *m_ptr = ref;
                auto id = m_ptr - m_vec->data().get();
                std::vector<T> old_items{ old_item };
                vector_changed_args<T> args{ vector_changed_action::replace, array_view<T>{ m_ptr, 1 }, (std::size_t)id, old_items, (std::size_t)id };
                m_vec->m_vector_changed(*m_vec, args);
                return *this;
            }
            reference& operator=(value_type&& ref) noexcept
            {
                auto old_item = std::move(*m_ptr);
                *m_ptr = std::move(ref);
                auto id = m_ptr - m_vec->data().get();
                std::vector<T> old_items{ old_item };
                vector_changed_args<T> args{ vector_changed_action::replace, array_view<T>{ m_ptr, 1 }, (std::size_t)id, old_items, (std::size_t)id };
                m_vec->m_vector_changed(*m_vec, args);
                return *this;
            }

            constexpr value_type* operator&() noexcept { return m_ptr; }
            constexpr value_type const* operator&() const noexcept { return m_ptr; }

            constexpr value_type& get() noexcept { return *m_ptr; }
            constexpr value_type const& get() const noexcept { return *m_ptr; }

            constexpr operator value_type&() noexcept { return *m_ptr; }
            constexpr operator value_type const&() const noexcept { return *m_ptr; }
        };
        friend class observable_vector::reference;
        using const_reference = reference const;

        class pointer
        {
        private:
            value_type* m_ptr;
            observable_vector* m_vec;

        public:
            constexpr pointer(value_type* ref, observable_vector* vec) noexcept : m_ptr(ref), m_vec(vec) {}
            constexpr pointer(value_type const* ref, observable_vector const* vec) noexcept : m_ptr(const_cast<value_type*>(ref)), m_vec(const_cast<observable_vector*>(vec)) {}

            constexpr reference operator*() noexcept { return { *m_ptr, *m_vec }; }
            constexpr const_reference operator*() const noexcept { return { *m_ptr, *m_vec }; }

            constexpr value_type* operator->() noexcept { return m_ptr; }
            constexpr value_type const* operator->() const noexcept { return m_ptr; }

            constexpr pointer& operator+=(std::ptrdiff_t diff) noexcept
            {
                m_ptr += diff;
                return *this;
            }

            constexpr pointer& operator-=(std::ptrdiff_t diff) noexcept
            {
                m_ptr -= diff;
                return *this;
            }

            friend constexpr pointer operator+(pointer const& p, std::ptrdiff_t diff) noexcept { return { p.m_ptr + diff, p.m_vec }; }
            friend constexpr pointer operator+(std::ptrdiff_t diff, pointer const& p) noexcept { return p + diff; }

            friend constexpr pointer operator-(pointer const& p, std::ptrdiff_t diff) noexcept { return { p.m_ptr - diff, p.m_vec }; }

            friend constexpr std::ptrdiff_t operator-(pointer const& lp, pointer const& rp) noexcept { return lp.m_ptr - rp.m_ptr; }

            constexpr value_type* get() noexcept { return m_ptr; }
            constexpr value_type const* get() const noexcept { return m_ptr; }

            constexpr operator value_type*() noexcept { return m_ptr; }
            constexpr operator value_type const*() const noexcept { return m_ptr; }
        };
        friend class observable_vector::pointer;
        using const_pointer = pointer const;

        struct const_iterator
        {
        public:
            using difference_type = typename __vector_type::difference_type;
            using value_type = typename observable_vector::value_type;
            using pointer = typename observable_vector::const_pointer;
            using reference = typename observable_vector::const_reference;
            using iterator_category = std::random_access_iterator_tag;

        private:
            using __iterator = typename __vector_type::const_iterator;
            __iterator m_it;
            observable_vector const* m_vec;

        public:
            const_iterator(__iterator it, observable_vector const& vec) : m_it(it), m_vec(&vec) {}
            const_iterator(const_iterator const& it) noexcept : m_it(it.m_it), m_vec(it.m_vec) {}

            const_iterator& operator=(const_iterator const& it) noexcept
            {
                m_it = it.m_it;
                m_vec = it.m_vec;
                return *this;
            }

            reference operator*() const { return { *m_it, *m_vec }; }

            value_type const* operator->() const { return m_it.operator->(); }

            const_iterator& operator++()
            {
                m_it++;
                return *this;
            }
            const_iterator operator++(int)
            {
                const_iterator it = *this;
                operator++();
                return it;
            }

            const_iterator& operator--()
            {
                m_it--;
                return *this;
            }
            const_iterator operator--(int)
            {
                const_iterator it = *this;
                operator--();
                return it;
            }

            const_iterator& operator+=(difference_type n)
            {
                m_it += n;
                return *this;
            }
            const_iterator& operator-=(difference_type n)
            {
                m_it -= n;
                return *this;
            }

            friend const_iterator operator+(const_iterator const& it, difference_type n) { return { it.m_it + n, *it.m_vec }; }
            friend const_iterator operator+(difference_type n, const_iterator const& it) { return it + n; }

            friend const_iterator operator-(const_iterator const& it, difference_type n) { return { it.m_it - n, *it.m_vec }; }

            friend difference_type operator-(const_iterator const& lhs, const_iterator const& rhs) { return lhs.m_it - rhs.m_it; }

            reference operator[](difference_type n) { return { m_it[n], *m_vec }; }
            reference const operator[](difference_type n) const { return { m_it[n], *m_vec }; }

            friend bool operator==(const_iterator const& lhs, const_iterator const& rhs)
            {
                assert(lhs.m_vec == rhs.m_vec);
                return lhs.m_it == rhs.m_it;
            }
            friend bool operator!=(const_iterator const& lhs, const_iterator const& rhs) { return !(lhs == rhs); }

            friend bool operator<(const_iterator const& lhs, const_iterator const& rhs)
            {
                assert(lhs.m_vec == rhs.m_vec);
                return lhs.m_it < rhs.m_it;
            }
            friend bool operator>(const_iterator const& lhs, const_iterator const& rhs) { return rhs < lhs; }
            friend bool operator<=(const_iterator const& lhs, const_iterator const& rhs) { return !(rhs < lhs); }
            friend bool operator>=(const_iterator const& lhs, const_iterator const& rhs) { return !(lhs < rhs); }

            operator __iterator() { return m_it; }
        };

        struct iterator
        {
        public:
            using difference_type = typename __vector_type::difference_type;
            using value_type = typename observable_vector::value_type;
            using pointer = typename observable_vector::pointer;
            using reference = typename observable_vector::reference;
            using iterator_category = std::random_access_iterator_tag;

        private:
            using __iterator = typename __vector_type::iterator;
            __iterator m_it;
            observable_vector* m_vec;

        public:
            iterator(__iterator it, observable_vector& vec) : m_it(it), m_vec(&vec) {}
            iterator(iterator const& it) noexcept : m_it(it.m_it), m_vec(it.m_vec) {}

            iterator& operator=(iterator const& it) noexcept
            {
                m_it = it.m_it;
                m_vec = it.m_vec;
                return *this;
            }

            reference operator*() { return { *m_it, *m_vec }; }
            reference const operator*() const { return { *m_it, *m_vec }; }

            value_type* operator->() { return m_it.operator->(); }
            value_type const* operator->() const { return m_it.operator->(); }

            iterator& operator++()
            {
                m_it++;
                return *this;
            }
            iterator operator++(int)
            {
                iterator it = *this;
                operator++();
                return it;
            }

            iterator& operator--()
            {
                m_it--;
                return *this;
            }
            iterator operator--(int)
            {
                iterator it = *this;
                operator--();
                return it;
            }

            iterator& operator+=(difference_type n)
            {
                m_it += n;
                return *this;
            }
            iterator& operator-=(difference_type n)
            {
                m_it -= n;
                return *this;
            }

            friend iterator operator+(iterator const& it, difference_type n) { return { it.m_it + n, *it.m_vec }; }
            friend iterator operator+(difference_type n, iterator const& it) { return it + n; }

            friend iterator operator-(iterator const& it, difference_type n) { return { it.m_it - n, *it.m_vec }; }

            friend difference_type operator-(iterator const& lhs, iterator const& rhs) { return lhs.m_it - rhs.m_it; }

            reference operator[](difference_type n) { return { m_it[n], *m_vec }; }
            reference const operator[](difference_type n) const { return { m_it[n], *m_vec }; }

            friend bool operator==(iterator const& lhs, iterator const& rhs)
            {
                assert(lhs.m_vec == rhs.m_vec);
                return lhs.m_it == rhs.m_it;
            }
            friend bool operator!=(iterator const& lhs, iterator const& rhs) { return !(lhs == rhs); }

            friend bool operator<(iterator const& lhs, iterator const& rhs)
            {
                assert(lhs.m_vec == rhs.m_vec);
                return lhs.m_it < rhs.m_it;
            }
            friend bool operator>(iterator const& lhs, iterator const& rhs) { return rhs < lhs; }
            friend bool operator<=(iterator const& lhs, iterator const& rhs) { return !(rhs < lhs); }
            friend bool operator>=(iterator const& lhs, iterator const& rhs) { return !(lhs < rhs); }

            operator const_iterator() { return { m_it, *m_vec }; }
            operator __iterator() { return m_it; }
        };

        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using size_type = typename __vector_type::size_type;
        using difference_type = typename __vector_type::difference_type;

    public:
        EVENT(vector_changed, std::reference_wrapper<observable_vector>, std::reference_wrapper<vector_changed_args<T>>)

    public:
        observable_vector() noexcept : m_vec() {}
        observable_vector(size_type count, value_type const& value) : m_vec(count, value) {}
        explicit observable_vector(size_type count) : m_vec(count) {}

        template <typename InputIt>
        observable_vector(InputIt first, InputIt last) : m_vec(first, last)
        {
        }

        observable_vector(observable_vector const& vec) : m_vec(vec.m_vec), m_vector_changed(vec.m_vector_changed) {}
        observable_vector(observable_vector&& vec) noexcept : m_vec(std::move(vec.m_vec)), m_vector_changed(std::move(vec.m_vector_changed)) {}

        observable_vector(std::initializer_list<value_type> init) : m_vec(init) {}

        observable_vector& operator=(observable_vector const& vec)
        {
            m_vec = vec.m_vec;
            m_vector_changed = vec.m_vector_changed;
            return *this;
        }
        observable_vector& operator=(observable_vector&& vec) noexcept
        {
            m_vec = std::move(vec.m_vec);
            m_vector_changed = std::move(vec.m_vector_changed);
            return *this;
        }

        observable_vector& operator=(std::initializer_list<value_type> ilist)
        {
            assign(ilist);
            return *this;
        }

        void assign(size_type count, value_type const& value)
        {
            auto old_vec = std::move(m_vec);
            m_vec.assign(count, value);
            vector_changed_args<T> args{ vector_changed_action::reset, m_vec, 0, old_vec, 0 };
            m_vector_changed(*this, args);
        }

        template <typename InputIt>
        void assign(InputIt first, InputIt last)
        {
            auto old_vec = std::move(m_vec);
            m_vec.assign(first, last);
            vector_changed_args<T> args{ vector_changed_action::reset, m_vec, 0, old_vec, 0 };
            m_vector_changed(*this, args);
        }

        void assign(std::initializer_list<value_type> ilist)
        {
            auto old_vec = std::move(m_vec);
            m_vec.assign(ilist);
            vector_changed_args<T> args{ vector_changed_action::reset, m_vec, 0, old_vec, 0 };
            m_vector_changed(*this, args);
        }

        reference at(size_type pos) { return { m_vec.at(pos), *this }; }
        const_reference at(size_type pos) const { return { m_vec.at(pos), *this }; }

        reference operator[](size_type pos) { return { m_vec[pos], *this }; }
        const_reference operator[](size_type pos) const { return { m_vec[pos], *this }; }

        reference front() { return { m_vec.front(), *this }; }
        const_reference front() const { return { m_vec.front(), *this }; }

        reference back() { return { m_vec.back(), *this }; }
        const_reference back() const { return { m_vec.back(), *this }; }

        pointer data() { return { m_vec.data(), this }; }
        const_pointer data() const { return { m_vec.data(), this }; }

        iterator begin() noexcept { return { m_vec.begin(), *this }; }
        const_iterator cbegin() const noexcept { return { m_vec.cbegin(), *this }; }
        const_iterator begin() const noexcept { return cbegin(); }

        iterator end() noexcept { return { m_vec.end(), *this }; }
        const_iterator cend() const noexcept { return { m_vec.cend(), *this }; }
        const_iterator end() const noexcept { return cend(); }

        reverse_iterator rbegin() noexcept { return std::reverse_iterator(end()); }
        const_reverse_iterator crbegin() const noexcept { return std::reverse_iterator(cend()); }
        const_reverse_iterator rbegin() const noexcept { return crbegin(); }

        reverse_iterator rend() noexcept { return std::reverse_iterator(begin()); }
        const_reverse_iterator crend() const noexcept { return std::reverse_iterator(cbegin()); }
        const_reverse_iterator rend() const noexcept { return crend(); }

        [[nodiscard]] bool empty() const noexcept { return m_vec.empty(); }
        size_type size() const noexcept { return m_vec.size(); }
        size_type max_size() const noexcept { return m_vec.max_size(); }
        void reserve(size_type new_cap) { m_vec.reserve(new_cap); }
        size_type capacity() const noexcept { return m_vec.capacity(); }
        void shrink_to_fit() { m_vec.shrink_to_fit(); }

        void clear() noexcept
        {
            auto old_vec = std::move(m_vec);
            m_vec.clear();
            vector_changed_args<T> args{ vector_changed_action::erase, {}, 0, old_vec, 0 };
            m_vector_changed(*this, args);
        }

        iterator insert(const_iterator pos, value_type const& value)
        {
            auto old_id = pos - begin();
            auto it = m_vec.insert(pos, value);
            vector_changed_args<T> args{ vector_changed_action::add, array_view<value_type>(std::addressof(*it), 1), (std::size_t)(it - m_vec.begin()), {}, (std::size_t)old_id };
            m_vector_changed(*this, args);
            return { it, *this };
        }
        iterator insert(const_iterator pos, value_type&& value)
        {
            auto old_id = pos - begin();
            auto it = m_vec.insert(pos, std::move(value));
            vector_changed_args<T> args{ vector_changed_action::add, array_view<value_type>(std::addressof(*it), 1), (std::size_t)(it - m_vec.begin()), {}, (std::size_t)old_id };
            m_vector_changed(*this, args);
            return { it, *this };
        }

        iterator insert(const_iterator pos, size_type count, value_type const& value)
        {
            auto old_id = pos - begin();
            auto it = m_vec.insert(pos, count, value);
            vector_changed_args<T> args{ vector_changed_action::add, array_view<value_type>(std::addressof(*it), count), (std::size_t)(it - m_vec.begin()), {}, (std::size_t)old_id };
            m_vector_changed(*this, args);
            return { it, *this };
        }

        template <typename InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last)
        {
            auto old_id = pos - begin();
            auto it = m_vec.insert(pos, first, last);
            vector_changed_args<T> args{ vector_changed_action::add, array_view<value_type>(std::addressof(*it), last - first), (std::size_t)(it - m_vec.begin()), {}, (std::size_t)old_id };
            m_vector_changed(*this, args);
            return { it, *this };
        }

        iterator insert(const_iterator pos, std::initializer_list<value_type> ilist)
        {
            return insert(pos, ilist.begin(), ilist.end());
        }

        iterator erase(const_iterator pos)
        {
            auto old_id = pos - begin();
            auto old_item = std::move(*pos);
            auto it = m_vec.erase(pos);
            vector_changed_args<T> args{ vector_changed_action::erase, {}, (std::size_t)(it - m_vec.begin()), { old_item }, (std::size_t)old_id };
            m_vector_changed(*this, args);
            return { it, *this };
        }

        iterator erase(const_iterator first, const_iterator last)
        {
            auto old_id = first - begin();
            std::vector<value_type> old_items(first, last);
            auto it = m_vec.erase(first, last);
            vector_changed_args<T> args{ vector_changed_action::erase, {}, (std::size_t)(it - m_vec.begin()), old_items, (std::size_t)old_id };
            m_vector_changed(*this, args);
            return { it, *this };
        }

        void push_back(value_type const& value)
        {
            auto old_id = m_vec.size();
            m_vec.push_back(value);
            vector_changed_args<T> args{ vector_changed_action::add, array_view<value_type>(std::addressof(*(m_vec.end() - 1)), 1), old_id, {}, old_id };
            m_vector_changed(*this, args);
        }

        void push_back(value_type&& value)
        {
            auto old_id = m_vec.size();
            m_vec.push_back(std::move(value));
            vector_changed_args<T> args{ vector_changed_action::add, array_view<value_type>(std::addressof(*(m_vec.end() - 1)), 1), old_id, {}, old_id };
            m_vector_changed(*this, args);
        }

        void pop_back()
        {
            auto old_id = m_vec.size() - 1;
            auto old_item = std::move(m_vec.back());
            m_vec.pop_back();
            std::vector<T> old_items{ old_item };
            vector_changed_args<T> args{ vector_changed_action::erase, {}, old_id, old_items, old_id };
            m_vector_changed(*this, args);
        }

        void resize(size_type count, value_type const& value)
        {
            if (count > m_vec.size())
            {
                auto old_id = m_vec.size();
                m_vec.resize(count, value);
                vector_changed_args<T> args{ vector_changed_action::add, array_view<value_type>(std::addressof(*(m_vec.begin() + old_id)), m_vec.size() - old_id), old_id, {}, old_id };
                m_vector_changed(*this, args);
            }
            else if (count < m_vec.size())
            {
                auto old_id = m_vec.size();
                std::vector<value_type> old_items(m_vec.begin() + count, m_vec.end());
                m_vec.resize(count, value);
                vector_changed_args<T> args{ vector_changed_action::erase, {}, old_id, old_items, old_id };
                m_vector_changed(*this, args);
            }
        }
        void resize(size_type count)
        {
            resize(count, {});
        }

        void swap(observable_vector& other) noexcept
        {
            m_vec.swap(other.m_vec);
            m_vector_changed.swap(other.m_vector_changed);
        }
    };

    template <typename T>
    class observable_vector_view
    {
    private:
        using __vector = observable_vector<T>;

    public:
        using value_type = typename __vector::value_type;
        using pointer = typename __vector::const_pointer;
        using const_pointer = typename __vector::const_pointer;
        using reference = typename __vector::const_reference;
        using const_reference = typename __vector::const_reference;
        using iterator = typename __vector::const_iterator;
        using const_iterator = typename __vector::const_iterator;
        using reverse_iterator = typename __vector::const_reverse_iterator;
        using const_reverse_iterator = typename __vector::const_reverse_iterator;
        using size_type = typename __vector::size_type;
        using difference_type = typename __vector::difference_type;

    private:
        __vector* m_vec{ nullptr };

    public:
        constexpr observable_vector_view() noexcept {}
        constexpr observable_vector_view(observable_vector<T>& obsv) noexcept : m_vec(&obsv) {}

        constexpr operator bool() const noexcept { return m_vec; }

        std::size_t add_vector_changed(std::function<void(observable_vector<T>&, vector_changed_args<T>&)>&& f) { return m_vec->add_vector_changed(std::move(f)); }
        void remove_vector_changed(std::size_t t) { m_vec->remove_vector_changed(t); }

        const_reference operator[](size_type pos) const { return m_vec->operator[](pos); }

        const_reference front() const { return m_vec->front(); }
        const_reference back() const { return m_vec->back(); }

        const_pointer data() const { return m_vec->data(); }

        const_iterator cbegin() const noexcept { return m_vec->cbegin(); }
        const_iterator cend() const noexcept { return m_vec->cend(); }
        iterator begin() const noexcept { return cbegin(); }
        iterator end() const noexcept { return cend(); }

        const_reverse_iterator crbegin() const noexcept { return m_vec->crbegin(); }
        const_reverse_iterator crend() const noexcept { return m_vec->crend(); }
        reverse_iterator rbegin() const noexcept { return crbegin(); }
        reverse_iterator rend() const noexcept { return crend(); }

        [[nodiscard]] bool empty() const noexcept { return m_vec->empty(); }
        size_type size() const noexcept { return m_vec->size(); }

        friend constexpr bool operator==(observable_vector_view<T> const& lhs, observable_vector_view<T> const& rhs) { return lhs.m_vec == rhs.m_vec; }
        friend constexpr bool operator!=(observable_vector_view<T> const& lhs, observable_vector_view<T> const& rhs) { return !(lhs == rhs); }
    };
} // namespace xaml

#endif // !XAML_OBSERVABLE_VECTOR_HPP
