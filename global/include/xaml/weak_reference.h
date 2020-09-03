#ifndef XAML_WEAK_REFERENCE_H
#define XAML_WEAK_REFERENCE_H

#ifdef __cplusplus
    #include <limits>
#endif // __cplusplus

#include <xaml/object.h>

XAML_CLASS(xaml_weak_reference, { 0xb93ad9e2, 0x3037, 0x45d3, { 0xbc, 0xdd, 0xf3, 0x18, 0xb1, 0xf6, 0x44, 0x90 } })

#define XAML_WEAK_REFERENCE_VTBL(type)         \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(resolve, type, xaml_guid XAML_CONST_REF, void**)

XAML_DECL_INTERFACE_(xaml_weak_reference, xaml_object)
{
    XAML_DECL_VTBL(xaml_weak_reference, XAML_WEAK_REFERENCE_VTBL);

#ifdef __cplusplus
    template <typename T>
    xaml_result XAML_CALL resolve(T * *ptr) noexcept
    {
        return resolve(xaml_type_guid_v<T>, reinterpret_cast<void**>(ptr));
    }
#endif // __cplusplus
};

XAML_CLASS(xaml_weak_reference_source, { 0x0e09bb36, 0x8879, 0x4eab, { 0x81, 0x51, 0x8f, 0x93, 0x8a, 0x67, 0xd3, 0xa0 } })

#define XAML_WEAK_REFERENCE_SOURCE_VTBL(type)  \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(get_weak_reference, type, xaml_weak_reference**)

XAML_DECL_INTERFACE_(xaml_weak_reference_source, xaml_object)
{
    XAML_DECL_VTBL(xaml_weak_reference_source, XAML_WEAK_REFERENCE_SOURCE_VTBL);
};

#ifdef __cplusplus
template <typename T, typename D, typename... Base>
struct __xaml_weak_reference_implement : __xaml_query_implement<__xaml_weak_reference_implement<T, D, Base...>, xaml_weak_reference, xaml_object>
{
    std::atomic<std::uint32_t> m_weak_ref_count{};
    std::atomic<std::uint32_t> m_strong_ref_count{};
    xaml_object* m_ptr{};

    std::uint32_t XAML_CALL add_ref() noexcept override;
    std::uint32_t XAML_CALL release() noexcept override;
    xaml_result XAML_CALL resolve(xaml_guid const&, void**) noexcept override;
};

template <typename T, typename D, typename... Base>
struct xaml_weak_implement : __xaml_query_implement<T, D, Base...>
{
    std::atomic<std::intptr_t> m_ref_count{ 1 };
    using __weakref_t = __xaml_weak_reference_implement<T, D, Base...>;

    virtual ~xaml_weak_implement() {}

    std::uint32_t XAML_CALL add_ref() noexcept override
    {
        std::intptr_t count;
        do
        {
            count = m_ref_count.load();
            if (count < 0)
            {
                auto control = reinterpret_cast<__weakref_t*>(count << 1);
                return ++(control->m_strong_ref_count);
            }
        } while (!m_ref_count.compare_exchange_strong(count, count + 1));
        return static_cast<std::uint32_t>(count + 1);
    }

    std::uint32_t XAML_CALL release() noexcept override
    {
        std::intptr_t count;
        do
        {
            count = m_ref_count.load();
            if (count < 0)
            {
                auto control = reinterpret_cast<__weakref_t*>(count << 1);
                std::uint32_t res = --(control->m_strong_ref_count);
                if (res == 0)
                {
                    delete static_cast<T*>(this);
                    control->m_ptr = nullptr;
                }
                return res;
            }
        } while (!m_ref_count.compare_exchange_strong(count, count - 1));
        std::uint32_t res = static_cast<std::uint32_t>(count - 1);
        if (res == 0)
        {
            delete static_cast<T*>(this);
        }
        return res;
    }

    xaml_result XAML_CALL get_weak_reference(xaml_weak_reference** ptr) noexcept
    {
        __weakref_t* control;
        std::intptr_t count = m_ref_count.load();
        if (count < 0)
        {
            control = reinterpret_cast<__weakref_t*>(count << 1);
        }
        else
        {
            control = new __weakref_t();
            control->m_ptr = this;
            control->m_strong_ref_count = static_cast<std::uint32_t>(m_ref_count.exchange((reinterpret_cast<std::intptr_t>(control) >> 1) | (std::numeric_limits<std::intptr_t>::min)()));
        }
        ++control->m_weak_ref_count;
        *ptr = control;
        return XAML_S_OK;
    }

    struct __weak_reference_source : xaml_inner_implement<__weak_reference_source, xaml_weak_implement<T, D, Base...>, xaml_weak_reference_source>
    {
        xaml_result XAML_CALL get_weak_reference(xaml_weak_reference** ptr) noexcept override { return this->m_outer->get_weak_reference(ptr); }
    } m_source;

    xaml_weak_implement() noexcept { m_source.m_outer = this; }

    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override
    {
        if (type == xaml_type_guid_v<xaml_weak_reference_source>)
        {
            this->add_ref();
            *ptr = static_cast<xaml_weak_reference_source*>(&m_source);
            return XAML_S_OK;
        }
        else
        {
            return __xaml_query_implement<T, D, Base...>::query(type, ptr);
        }
    }
};

template <typename T, typename D, typename... Base>
inline std::uint32_t __xaml_weak_reference_implement<T, D, Base...>::add_ref() noexcept
{
    return ++m_weak_ref_count;
}

template <typename T, typename D, typename... Base>
inline std::uint32_t __xaml_weak_reference_implement<T, D, Base...>::release() noexcept
{
    std::uint32_t res = --(m_weak_ref_count);
    if (res == 0)
    {
        auto impl = static_cast<xaml_weak_implement<T, D, Base...>*>(m_ptr);
        auto ref = m_strong_ref_count.load();
        if (ref > 0)
        {
            impl->m_ref_count = ref;
        }
        delete this;
    }
    return res;
}

template <typename T, typename D, typename... Base>
inline xaml_result __xaml_weak_reference_implement<T, D, Base...>::resolve(xaml_guid const& type, void** ptr) noexcept
{
    if (m_ptr)
    {
        ++m_strong_ref_count;
        xaml_result hr = m_ptr->query(type, ptr);
        m_ptr->release();
        return hr;
    }
    else
    {
        *ptr = nullptr;
        return XAML_S_OK;
    }
}
#endif // __cplusplus

#endif // !XAML_WEAK_REFERENCE_H
