#ifndef XAML_EVENT_H
#define XAML_EVENT_H

#ifdef __cplusplus
    #include <map>
    #include <xaml/ptr.hpp>
#endif // __cplusplus

#include <xaml/delegate.h>
#include <xaml/object.h>

XAML_TYPE_BASE(xaml_event_2, { 0x84577f0b, 0xaf47, 0x4f60, { 0x8e, 0xe6, 0x69, 0x6b, 0x2b, 0xdd, 0xca, 0x7f } })

#define XAML_EVENT_2_VTBL(type, TA1N, TA1I, TA2N, TA2I)                           \
    XAML_VTBL_INHERIT(XAML_DELEGATE_2_VTBL(type, TA1N, TA1I, TA2N, TA2I));        \
    XAML_METHOD(add, type, XAML_DELEGATE_2_NAME(TA1N, TA2N)*, XAML_STD int32_t*); \
    XAML_METHOD(remove, type, XAML_STD int32_t)

#ifdef __cplusplus
template <typename TS, typename TE>
struct xaml_event : xaml_delegate<TS, TE>
{
    XAML_EVENT_2_VTBL(xaml_event, TS, xaml_interface_t<TS>, TE, xaml_interface_t<TE>);

    template <typename F>
    xaml_result XAML_CALL add(F&& func, std::int32_t* ptoken) noexcept
    {
        xaml_ptr<xaml_delegate<TS, TE>> handler;
        XAML_RETURN_IF_FAILED((xaml_delegate_new<TS, TE>(std::forward<F>(func), &handler)));
        return add(handler.get(), ptoken);
    }
};

template <typename TS, typename TE>
struct xaml_base<xaml_event<TS, TE>>
{
    using type = xaml_delegate<TS, TE>;
};

template <typename TS, typename TE>
struct xaml_type_guid<xaml_event<TS, TE>>
{
    static constexpr xaml_guid value = xaml_guid_xaml_event_2;
};

    #define XAML_EVENT_2_NAME(type1, type2) xaml_event<type1, type2>

    #define __XAML_EVENT_2_TYPE(type1, type2) typedef xaml_event<type1, type2> xaml_event_2__##type1##__##type2;
#else
    #define XAML_EVENT_2_NAME(type1, type2) xaml_event_2__##type1##__##type2

    #define __XAML_EVENT_2_TYPE(type1_name, type1_interface, type2_name, type2_interface) \
        XAML_DECL_INTERFACE_T_(xaml_event_2, type1_name##__##type2_name, XAML_EVENT_2_VTBL, type1_name, type1_interface, type2_name, type2_interface)
#endif // __cplusplus
#define XAML_EVENT_2_TYPE(type1, type2) __XAML_EVENT_2_TYPE(type1, type2)

#ifdef __cplusplus
template <typename TS, typename TE>
struct __xaml_event_implement : xaml_implement<__xaml_event_implement<TS, TE>, xaml_event<TS, TE>>
{
    std::atomic<std::int32_t> m_index{ 0 };
    std::map<std::int32_t, xaml_ptr<xaml_delegate<TS, TE>>> m_callbacks{};

    xaml_result XAML_CALL add(xaml_delegate<TS, TE>* handler, std::int32_t* ptoken) noexcept override
    {
        std::int32_t token = ++m_index;
        try
        {
            m_callbacks.emplace(token, handler);
            *ptoken = token;
            return XAML_S_OK;
        }
        XAML_CATCH_RETURN()
    }

    xaml_result XAML_CALL remove(std::int32_t token) noexcept override
    {
        auto it = m_callbacks.find(token);
        if (it == m_callbacks.end()) return XAML_E_KEYNOTFOUND;
        m_callbacks.erase(it);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL invoke(xaml_interface_t<TS> sender, xaml_interface_t<TE> e) noexcept override
    {
        for (auto& p : m_callbacks)
        {
            XAML_RETURN_IF_FAILED(p.second->invoke(sender, e));
        }
        return XAML_S_OK;
    }
};

template <typename TS, typename TE>
xaml_result XAML_CALL xaml_event_new(xaml_event<TS, TE>** ptr) noexcept
{
    return xaml_object_new<__xaml_event_implement<TS, TE>>(ptr);
}
#endif // __cplusplus

XAML_CLASS(xaml_event_args, { 0xb2998082, 0x5a53, 0x4ab0, { 0xa3, 0xc4, 0x2c, 0x6a, 0x90, 0xf1, 0x23, 0x4a } })

#define XAML_EVENT_ARGS_VTBL(type) XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type))

XAML_DECL_INTERFACE_(xaml_event_args, xaml_object)
{
    XAML_DECL_VTBL(xaml_event_args, XAML_EVENT_ARGS_VTBL);
};

EXTERN_C XAML_API xaml_result XAML_CALL xaml_event_args_empty(xaml_event_args**) XAML_NOEXCEPT;

#endif // !XAML_EVENT_H
