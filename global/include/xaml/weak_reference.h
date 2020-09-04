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
struct __xaml_weak_reference_implement : xaml_implement<__xaml_weak_reference_implement, xaml_weak_reference>
{
    std::atomic<std::uint32_t> m_strong_ref_count{};
    xaml_object* m_ptr{};

    __xaml_weak_reference_implement() { this->m_ref_count = 2; }

    xaml_result XAML_CALL resolve(xaml_guid const& type, void** ptr) noexcept override
    {
        *ptr = nullptr;
        std::uint32_t ref;
        do
        {
            ref = m_strong_ref_count.load();
            if (ref == 0) return XAML_S_OK;
        } while (!m_strong_ref_count.compare_exchange_strong(ref, ref + 1));
        xaml_result hr = m_ptr->query(type, ptr);
        m_ptr->release();
        return hr;
    }
};

template <typename T, typename D>
struct __xaml_weak_implement : __xaml_query_implement<T, D>
{
    std::atomic<std::intptr_t> m_ref_count{ 1 };

    using __weakref_t = __xaml_weak_reference_implement;

    static constexpr std::intptr_t __encode(__weakref_t* ptr) noexcept
    {
        return (reinterpret_cast<std::intptr_t>(ptr) >> 1) | (std::numeric_limits<std::intptr_t>::min)();
    }

    static constexpr __weakref_t* __decode(std::intptr_t count) noexcept
    {
        return reinterpret_cast<__weakref_t*>(count << 1);
    }

    static constexpr bool __is_ptr(std::intptr_t count) noexcept
    {
        return count < 0;
    }

    ~__xaml_weak_implement() override
    {
        auto count = m_ref_count.load();
        if (__is_ptr(count))
        {
            auto control = __decode(count);
            control->release();
        }
    }

    std::uint32_t XAML_CALL add_ref() noexcept override
    {
        std::intptr_t count;
        do
        {
            count = m_ref_count.load();
            if (__is_ptr(count))
            {
                auto control = __decode(count);
                return ++(control->m_strong_ref_count);
            }
        } while (!m_ref_count.compare_exchange_strong(count, count + 1));
        return static_cast<std::uint32_t>(count + 1);
    }

    inline std::uint32_t __release_impl() noexcept
    {
        std::intptr_t count;
        do
        {
            count = m_ref_count.load();
            if (__is_ptr(count))
            {
                auto control = reinterpret_cast<__weakref_t*>(count << 1);
                return --(control->m_strong_ref_count);
            }
        } while (!m_ref_count.compare_exchange_strong(count, count - 1));
        return static_cast<std::uint32_t>(count - 1);
    }

    std::uint32_t XAML_CALL release() noexcept override
    {
        std::uint32_t res = __release_impl();
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
        if (__is_ptr(count))
        {
            control = __decode(count);
            control->add_ref();
            *ptr = control;
            return XAML_S_OK;
        }
        control = new (std::nothrow) __weakref_t();
        if (control == nullptr) return XAML_E_OUTOFMEMORY;
        std::intptr_t new_count = __encode(control);
        while (true)
        {
            control->m_ptr = this;
            control->m_strong_ref_count = static_cast<std::uint32_t>(count);
            if (m_ref_count.compare_exchange_strong(count, new_count))
            {
                *ptr = control;
                return XAML_S_OK;
            }
            else if (__is_ptr(count))
            {
                delete control;
                control = __decode(count);
                control->add_ref();
                *ptr = control;
                return XAML_S_OK;
            }
        }
    }
};

template <typename T, typename D>
struct __xaml_weak_reference_source_implement : xaml_inner_implement<__xaml_weak_reference_source_implement<T, D>, __xaml_weak_implement<T, D>, xaml_weak_reference_source>
{
    xaml_result XAML_CALL get_weak_reference(xaml_weak_reference** ptr) noexcept override { return this->m_outer->get_weak_reference(ptr); }
};

template <typename T1, typename T2>
struct __xaml_weak_compressed_implement : T1
{
    T2 m_source;

    __xaml_weak_compressed_implement() noexcept { m_source.m_outer = this; }

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
            return T1::query(type, ptr);
        }
    }
};

template <typename T1>
struct __xaml_weak_compressed_implement<T1, void> : T1
{
};

template <typename T, typename D>
struct xaml_weak_implement
    : __xaml_weak_compressed_implement<
          __xaml_weak_implement<T, D>,
          std::conditional_t<
              std::is_base_of_v<xaml_weak_reference_source, D>,
              void,
              __xaml_weak_reference_source_implement<T, D>>>
{
};
#endif // __cplusplus

#endif // !XAML_WEAK_REFERENCE_H
