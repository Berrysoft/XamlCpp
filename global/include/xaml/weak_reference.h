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
struct __xaml_weak_reference_control
{
    std::atomic<std::uint32_t> weak_ref_count;
    std::atomic<std::uint32_t> strong_ref_count;
    std::atomic<xaml_object*> ptr;
};

template <typename T, typename D, typename... Base>
xaml_result XAML_CALL __xaml_weak_reference_new(__xaml_weak_reference_control*, xaml_weak_reference**) noexcept;

template <typename T, typename D, typename... Base>
struct xaml_weak_implement : __xaml_query_implement<T, D, Base...>
{
    std::atomic<std::intptr_t> m_ref_count{ 1 };

    virtual ~xaml_weak_implement() {}

    std::uint32_t XAML_CALL add_ref() noexcept override
    {
        std::intptr_t count;
        do
        {
            count = m_ref_count.load();
            if (count < 0)
            {
                auto control = reinterpret_cast<__xaml_weak_reference_control*>(count << 1);
                return ++(control->strong_ref_count);
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
                auto control = reinterpret_cast<__xaml_weak_reference_control*>(count << 1);
                std::uint32_t res = --(control->strong_ref_count);
                if (res == 0)
                {
                    delete static_cast<T*>(this);
                    control->ptr = nullptr;
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
        __xaml_weak_reference_control* control;
        std::intptr_t count = m_ref_count.load();
        if (count < 0)
        {
            control = reinterpret_cast<__xaml_weak_reference_control*>(count << 1);
        }
        else
        {
            control = new __xaml_weak_reference_control();
            control->ptr = this;
            control->strong_ref_count = (std::uint32_t)m_ref_count.exchange(((std::intptr_t)control >> 1) | (std::numeric_limits<std::intptr_t>::min)());
        }
        return __xaml_weak_reference_new<T, D, Base...>(control, ptr);
    }

    void XAML_CALL release_weak_reference(__xaml_weak_reference_control* pcontrol) noexcept
    {
        if (pcontrol->strong_ref_count.load() > 0)
        {
            std::intptr_t count = m_ref_count.load();
            if (count < 0)
            {
                m_ref_count = pcontrol->strong_ref_count;
                delete pcontrol;
            }
        }
        else
        {
            delete pcontrol;
        }
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
struct __xaml_weak_reference_impl : __xaml_query_implement<__xaml_weak_reference_impl<T, D, Base...>, xaml_weak_reference, xaml_object>
{
    std::atomic<__xaml_weak_reference_control*> m_control{};

    __xaml_weak_reference_impl(__xaml_weak_reference_control* pcontrol) noexcept : m_control(pcontrol)
    {
        add_ref();
    }

    std::uint32_t XAML_CALL add_ref() noexcept override
    {
        return ++(m_control.load()->weak_ref_count);
    }

    std::uint32_t XAML_CALL release() noexcept override
    {
        auto control = m_control.load();
        std::uint32_t res = --(control->weak_ref_count);
        if (res == 0)
        {
            static_cast<xaml_weak_implement<T, D, Base...>*>(control->ptr.load())->release_weak_reference(control);
        }
        return res;
    }

    xaml_result XAML_CALL resolve(xaml_guid const& type, void** ptr) noexcept override
    {
        xaml_object* real_ptr = m_control.load()->ptr.load();
        if (real_ptr)
        {
            return real_ptr->query(type, ptr);
        }
        else
        {
            *ptr = nullptr;
            return XAML_S_OK;
        }
    }
};

template <typename T, typename D, typename... Base>
xaml_result XAML_CALL __xaml_weak_reference_new(__xaml_weak_reference_control* pcontrol, xaml_weak_reference** ptr) noexcept
{
    return xaml_object_new<__xaml_weak_reference_impl<T, D, Base...>>(ptr, pcontrol);
}
#endif // __cplusplus

#endif // !XAML_WEAK_REFERENCE_H
