#ifndef XAML_IMPLEMENT_HPP
#define XAML_IMPLEMENT_HPP

#include <atomic>
#include <xaml/guid.h>
#include <xaml/object.h>

template <typename T, typename D, typename... Base>
struct xaml_implement : D
{
protected:
    std::atomic<std::size_t> m_ref_count{ 1 };

public:
    std::size_t XAML_CALL add_ref() noexcept override { return ++m_ref_count; }

    std::size_t XAML_CALL release() noexcept override
    {
        std::size_t res = --m_ref_count;
        if (!res)
        {
            delete static_cast<T*>(this);
            return 0;
        }
        else
        {
            return res;
        }
    }

private:
    template <typename... B>
    struct query_many_impl;

    template <typename B1, typename... B>
    struct query_many_impl<B1, B...>
    {
        xaml_result operator()(xaml_implement<T, D, Base...>* self, xaml_guid const& type, xaml_object** ptr) const noexcept
        {
            if (type == xaml_type_guid_v<B1>)
            {
                *ptr = static_cast<B1*>(self);
                self->add_ref();
                return 0;
            }
            else
            {
                return query_many_impl<B...>{}(self, type, ptr);
            }
        }
    };

    template <>
    struct query_many_impl<>
    {
        xaml_result operator()(xaml_implement<T, D, Base...>*, xaml_guid const&, xaml_object**) const noexcept { return 1; }
    };

public:
    xaml_result XAML_CALL query(xaml_guid const& type, xaml_object** ptr) noexcept override
    {
        return query_many_impl<D, Base...>{}(this, type, ptr);
    }
};

template <typename D, typename T, typename... Args>
inline xaml_result xaml_object_new(T** ptr, Args&&... args) noexcept
{
    T* res = new (std::nothrow) D(std::forward<Args>(args)...);
    if (!res) return XAML_E_OUTOFMEMORY;
    *ptr = res;
    return XAML_S_OK;
}

#endif // !XAML_IMPLEMENT_HPP
