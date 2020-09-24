#ifndef XAML_EVENT_H
#define XAML_EVENT_H

#ifdef __cplusplus
    #include <map>
    #include <xaml/ptr.hpp>
#endif // __cplusplus

#include <xaml/delegate.h>
#include <xaml/object.h>

__XAML_TYPE_NAME_BASE(xaml_event, { 0x84577f0b, 0xaf47, 0x4f60, { 0x8e, 0xe6, 0x69, 0x6b, 0x2b, 0xdd, 0xca, 0x7f } })

#define XAML_EVENT_T_T_VTBL(type, TA1N, TA1I, TA2N, TA2I)                              \
    XAML_VTBL_INHERIT(XAML_DELEGATE_T_T_VTBL(type, TA1N, TA1I, TA2N, TA2I));           \
    XAML_METHOD(add, type, xaml_delegate__##TA1N##____##TA2N##__*, XAML_STD int32_t*); \
    XAML_METHOD(remove, type, XAML_STD int32_t)

#ifdef __cplusplus
template <typename TS, typename TE>
struct xaml_event : xaml_delegate<TS, TE>
{
    XAML_DECL_VTBL_T(xaml_event<TS, TE>, XAML_EVENT_T_T_VTBL, TS, xaml_interface_t<TS>, TE, xaml_interface_t<TE>);
};

template <typename TS, typename TE>
struct xaml_base<xaml_event<TS, TE>>
{
    using base = xaml_delegate<TS, TE>;
};

template <typename TS, typename TE>
struct xaml_type_guid<xaml_event<TS, TE>>
{
    static constexpr xaml_guid value = xaml_guid_xaml_event;
};

    #define XAML_EVENT_T_T_TYPE(type1, type2) typedef xaml_event<type1, type2> xaml_event__##type1##____##type2##__;

    #define XAML_EVENT_T_O_T_O_TYPE(type1, type2) XAML_EVENT_T_T_TYPE(type1, type2)
#else
    #define XAML_EVENT_T_T_TYPE(type1_name, type1_interface, type2_name, type2_interface) \
        XAML_DECL_INTERFACE_T_(xaml_event, type1_name##____##type2_name, XAML_EVENT_T_T_VTBL, type1_name, type1_interface, type2_name, type2_interface)

    #define XAML_EVENT_T_O_T_O_TYPE(type1, type2) XAML_EVENT_T_T_TYPE(type1, type1*, type2, type2*)
#endif // __cplusplus

#ifdef __cplusplus
template <typename TS, typename TE>
struct __xaml_event_implement : xaml_implement<__xaml_event_implement<TS, TE>, xaml_delegate<TS, TE>>
{
    std::atomic<std::int32_t> m_index{ 0 };
    std::map<std::int32_t, xaml_ptr<xaml_delegate<TS, TE>>> m_callbacks{};

    xaml_result XAML_CALL add(xaml_delegate<TS, TE>* handler, std::int32_t* ptoken) noexcept override
    {
        std::int32_t token = ++index;
        try
        {
            m_callbacks.emplace(token, handler);
            *ptoken = token;
            return XAML_S_OK;
        }
        XAML_CATCH_RETURN()
    }

    xaml_result XAML_CALL remove(int32_t token) noexcept override
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
#endif // __cplusplus

#endif // !XAML_EVENT_H
