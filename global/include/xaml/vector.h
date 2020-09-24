#ifndef XAML_VECTOR_H
#define XAML_VECTOR_H

#ifdef __cplusplus
    #include <vector>
    #include <xaml/ptr.hpp>
#endif // __cplusplus

#include <xaml/enumerable.h>

__XAML_TYPE_NAME_BASE(xaml_vector_view, { 0x8960a280, 0xddbb, 0x4b5b, { 0xb4, 0xeb, 0x27, 0x6d, 0xd3, 0x90, 0x6e, 0xd6 } })

#define XAML_VECTOR_VIEW_T_VTBL(type, TN, TI)                \
    XAML_VTBL_INHERIT(XAML_ENUMERABLE_T_VTBL(type, TN, TI)); \
    XAML_METHOD(get_at, type, XAML_STD int32_t, TI*);        \
    XAML_METHOD(index_of, type, TI, XAML_STD int32_t*);      \
    XAML_METHOD(get_size, type, XAML_STD int32_t*)

#ifdef __cplusplus
XAML_DECL_INTERFACE_T_(xaml_vector_view, xaml_enumerable<T>, XAML_VECTOR_VIEW_T_VTBL)

    #define XAML_VECTOR_VIEW_T_TYPE(type) typedef xaml_vector_view<type> xaml_vector_view__##type##__;

    #define XAML_VECTOR_VIEW_T_V_TYPE(type) XAML_VECTOR_VIEW_T_TYPE(type)
    #define XAML_VECTOR_VIEW_T_O_TYPE(type) XAML_VECTOR_VIEW_T_TYPE(type)
#else
    #define XAML_VECTOR_VIEW_T_TYPE(type_name, type_interface) \
        XAML_DECL_INTERFACE_T_(xaml_vector_view, type_name, XAML_VECTOR_VIEW_T_VTBL, type_name, type_interface)

    #define XAML_VECTOR_VIEW_T_V_TYPE(type) XAML_VECTOR_VIEW_T_TYPE(type, type)
    #define XAML_VECTOR_VIEW_T_O_TYPE(type) XAML_VECTOR_VIEW_T_TYPE(type, type*)
#endif // __cplusplus

__XAML_TYPE_NAME_BASE(xaml_vector, { 0xad5e7c14, 0x969d, 0x4e76, { 0x97, 0x6e, 0xc3, 0x17, 0xb4, 0x41, 0x12, 0x5e } })

#define XAML_VECTOR_T_VTBL(type, TN, TI)                      \
    XAML_VTBL_INHERIT(XAML_VECTOR_VIEW_T_VTBL(type, TN, TI)); \
    XAML_METHOD(set_at, type, XAML_STD int32_t, TI);          \
    XAML_METHOD(append, type, TI);                            \
    XAML_METHOD(insert_at, type, XAML_STD int32_t, TI);       \
    XAML_METHOD(remove_at, type, XAML_STD int32_t);           \
    XAML_METHOD(remove_at_end, type);                         \
    XAML_METHOD(clear, type)

#ifdef __cplusplus
XAML_DECL_INTERFACE_T_(xaml_vector, xaml_vector_view<T>, XAML_VECTOR_T_VTBL)

    #define XAML_VECTOR_T_TYPE(type) typedef xaml_vector<type> xaml_vector__##type##__;

    #define XAML_VECTOR_T_V_TYPE(type) XAML_VECTOR_T_TYPE(type)
    #define XAML_VECTOR_T_O_TYPE(type) XAML_VECTOR_T_TYPE(type)
#else
    #define XAML_VECTOR_T_TYPE(type_name, type_interface) \
        XAML_DECL_INTERFACE_T_(xaml_vector, type_name, XAML_VECTOR_T_VTBL, type_name, type_interface)

    #define XAML_VECTOR_T_V_TYPE(type) XAML_VECTOR_T_TYPE(type, type)
    #define XAML_VECTOR_T_O_TYPE(type) XAML_VECTOR_T_TYPE(type, type*)
#endif // __cplusplus

#ifdef __cplusplus
template <typename T>
struct __xaml_vector_enumerator_implement : xaml_implement<__xaml_vector_enumerator_implement<T>, xaml_enumerator<T>>
{
    using inner_iterator_type = typename std::vector<xaml_interface_var_t<T>>::const_iterator;

    inner_iterator_type m_begin, m_end;
    bool m_init;

    __xaml_vector_enumerator_implement(inner_iterator_type begin, inner_iterator_type end) noexcept
        : m_begin(begin), m_end(end), m_init(false) {}

    xaml_result XAML_CALL move_next(bool* pb) noexcept override
    {
        if (!m_init)
        {
            m_init = true;
        }
        else
        {
            ++m_begin;
        }
        *pb = m_begin != m_end;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_current(xaml_interface_t<T>* ptr) noexcept override
    {
        if (m_begin == m_end) return XAML_E_OUTOFBOUNDS;
        return xaml_interface_assign<T>(*m_begin, ptr);
    }
};

template <typename T>
struct __xaml_vector_implement : xaml_implement<__xaml_vector_implement<T>, xaml_vector<T>>
{
    using inner_vector_type = std::vector<xaml_interface_var_t<T>>;

    inner_vector_type m_vec{};

    __xaml_vector_implement(inner_vector_type&& vec) noexcept : m_vec(std::move(vec)) {}

    xaml_result XAML_CALL get_size(int32_t* psize) noexcept override
    {
        *psize = static_cast<int32_t>(m_vec.size());
        return XAML_S_OK;
    }

    xaml_result XAML_CALL index_of(xaml_interface_t<T> value, int32_t* pindex) noexcept override
    {
        for (size_t i = 0; i < m_vec.size(); i++)
        {
            if (m_vec[i] == value)
            {
                *pindex = static_cast<int32_t>(i);
                return XAML_S_OK;
            }
        }
        *pindex = -1;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_at(int32_t index, xaml_interface_t<T>* ptr) noexcept override
    {
        if (index < 0 || index >= static_cast<int32_t>(m_vec.size())) return XAML_E_OUTOFBOUNDS;
        return xaml_interface_assign<T>(m_vec[index], ptr);
    }

    xaml_result XAML_CALL set_at(int32_t index, xaml_interface_t<T> obj) noexcept override
    {
        if (index < 0 || index >= static_cast<int32_t>(m_vec.size())) return XAML_E_OUTOFBOUNDS;
        m_vec[index] = obj;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL append(xaml_interface_t<T> obj) noexcept override
    try
    {
        m_vec.push_back(obj);
        return XAML_S_OK;
    }
    XAML_CATCH_RETURN()

    xaml_result XAML_CALL insert_at(int32_t index, xaml_interface_t<T> obj) noexcept override
    try
    {
        if (index < 0 || index >= static_cast<int32_t>(m_vec.size())) return XAML_E_OUTOFBOUNDS;
        m_vec.insert(m_vec.begin() + index, obj);
        return XAML_S_OK;
    }
    XAML_CATCH_RETURN()

    xaml_result XAML_CALL remove_at(int32_t index) noexcept override
    {
        if (index < 0 || index >= static_cast<int32_t>(m_vec.size())) return XAML_E_OUTOFBOUNDS;
        m_vec.erase(m_vec.begin() + index);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL remove_at_end() noexcept override
    {
        if (m_vec.empty()) return XAML_E_OUTOFBOUNDS;
        m_vec.pop_back();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL clear() noexcept override
    {
        m_vec.clear();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_enumerator(xaml_enumerator<T>** ptr) noexcept override
    {
        return xaml_object_new<__xaml_vector_enumerator_implement<T>>(ptr, m_vec.begin(), m_vec.end());
    }
};

template <typename T>
xaml_result XAML_CALL xaml_vector_new(std::vector<xaml_interface_var_t<T>>&& vec, xaml_vector<T>** ptr) noexcept
{
    return xaml_object_new<__xaml_vector_implement<T>>(ptr, std::move(vec));
}

template <typename T>
xaml_result XAML_CALL xaml_vector_new(xaml_vector<T>** ptr) noexcept
try
{
    return xaml_vector_new<T>({}, ptr);
}
XAML_CATCH_RETURN()
#endif // __cplusplus

#endif // !XAML_VECTOR_H
