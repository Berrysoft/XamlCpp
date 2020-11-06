#ifndef XAML_MAP_H
#define XAML_MAP_H

#ifdef __cplusplus
    #include <functional>
    #include <unordered_map>
    #include <xaml/box.h>
    #include <xaml/ptr.hpp>
    #ifdef XAML_SUPPORT_FUNCTION2
        #include <function2/function2.hpp>
    #endif // XAML_SUPPORT_FUNCTION2
#endif // __cplusplus

#include <xaml/enumerable.h>
#include <xaml/string.h>

XAML_TYPE_BASE(xaml_key_value_pair_2, { 0x41ea97bb, 0xda95, 0x430c, { 0xa8, 0x1b, 0xdb, 0x06, 0x61, 0x9f, 0x7e, 0x96 } })

#define XAML_KEY_VALUE_PAIR_2_VTBL(type, TKeyN, TKeyI, TValueN, TValueI) \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                           \
    XAML_METHOD(get_key, type, TKeyI*);                                  \
    XAML_METHOD(get_value, type, TValueI*)

#ifdef __cplusplus
template <typename TKey, typename TValue>
struct xaml_key_value_pair : xaml_object
{
    XAML_KEY_VALUE_PAIR_2_VTBL(xaml_key_value_pair, TKey, xaml_interface_t<TKey>, TValue, xaml_interface_t<TValue>);
};

template <typename TKey, typename TValue>
struct xaml_base<xaml_key_value_pair<TKey, TValue>>
{
    using type = xaml_object;
};

template <typename TKey, typename TValue>
struct xaml_type_guid<xaml_key_value_pair<TKey, TValue>>
{
    static constexpr xaml_guid value = xaml_guid_xaml_key_value_pair_2;
};

    #define XAML_KEY_VALUE_PAIR_2_NAME(tkey, tvalue) xaml_key_value_pair<tkey, tvalue>

    #define __XAML_KEY_VALUE_PAIR_2_TYPE(tkey, tvalue) typedef xaml_key_value_pair<tkey, tvalue> xaml_key_value_pair_2__##tkey##__##tvalue;
#else
    #define XAML_KEY_VALUE_PAIR_2_NAME(tkey, tvalue) xaml_key_value_pair_2__##tkey##__##tvalue

    #define __XAML_KEY_VALUE_PAIR_2_TYPE(tkey_name, tkey_interface, tvalue_name, tvalue_interface) \
        XAML_DECL_INTERFACE_T_(xaml_key_value_pair_2, tkey_name##__##tvalue_name, XAML_KEY_VALUE_PAIR_2_VTBL, tkey_name, tkey_interface, tvalue_name, tvalue_interface)
#endif // __cplusplus
#define XAML_KEY_VALUE_PAIR_2_TYPE(tkey, tvalue) __XAML_KEY_VALUE_PAIR_2_TYPE(tkey, tvalue)

#ifdef __cplusplus
template <typename TKey, typename TValue>
struct __xaml_key_value_pair_implement : xaml_implement<__xaml_key_value_pair_implement<TKey, TValue>, xaml_key_value_pair<TKey, TValue>>
{
    xaml_interface_var_t<TKey> m_key;
    xaml_interface_var_t<TValue> m_value;

    __xaml_key_value_pair_implement(xaml_interface_t<TKey> key, xaml_interface_t<TValue> value) noexcept : m_key(std::move(key)), m_value(std::move(value)) {}

    xaml_result XAML_CALL get_key(xaml_interface_t<TKey>* ptr) noexcept override
    {
        return xaml_interface_assign<TKey>(m_key, ptr);
    }

    xaml_result XAML_CALL get_value(xaml_interface_t<TValue>* ptr) noexcept override
    {
        return xaml_interface_assign<TValue>(m_value, ptr);
    }
};

template <typename TKey, typename TValue>
xaml_result XAML_CALL xaml_key_value_pair_new(xaml_interface_t<TKey> key, xaml_interface_t<TValue> value, xaml_key_value_pair<TKey, TValue>** ptr) noexcept
{
    return xaml_object_new<__xaml_key_value_pair_implement<TKey, TValue>>(ptr, std::move(key), std::move(value));
}
#endif // __cplusplus

XAML_TYPE_BASE(xaml_map_view_2, { 0x15549c22, 0x40d1, 0x4af1, { 0xad, 0x81, 0x0d, 0xd3, 0xda, 0x1a, 0x87, 0xba } })

#define XAML_MAP_VIEW_2_VTBL(type, TKeyN, TKeyI, TValueN, TValueI)                                                                            \
    XAML_VTBL_INHERIT(XAML_ENUMERABLE_1_VTBL(type, XAML_KEY_VALUE_PAIR_2_NAME(TKeyN, TValueN), XAML_KEY_VALUE_PAIR_2_NAME(TKeyN, TValueN)*)); \
    XAML_METHOD(lookup, type, TKeyI, TValueI*);                                                                                               \
    XAML_METHOD(has_key, type, TKeyI, bool*);                                                                                                 \
    XAML_METHOD(get_size, type, XAML_STD int32_t*)

#ifdef __cplusplus
template <typename TKey, typename TValue>
struct xaml_map_view : xaml_enumerable<xaml_key_value_pair<TKey, TValue>>
{
    XAML_MAP_VIEW_2_VTBL(xaml_map_view, TKey, xaml_interface_t<TKey>, TValue, xaml_interface_t<TValue>);
};

template <typename TKey, typename TValue>
struct xaml_base<xaml_map_view<TKey, TValue>>
{
    using type = xaml_enumerable<xaml_key_value_pair<TKey, TValue>>;
};

template <typename TKey, typename TValue>
struct xaml_type_guid<xaml_map_view<TKey, TValue>>
{
    static constexpr xaml_guid value = xaml_guid_xaml_map_view_2;
};

    #define XAML_MAP_VIEW_2_NAME(tkey, tvalue) xaml_map_view<tkey, tvalue>

    #define __XAML_MAP_VIEW_2_TYPE(tkey, tvalue) typedef xaml_map_view<tkey, tvalue> xaml_map_view_2__##tkey##__##tvalue;
#else
    #define XAML_MAP_VIEW_2_NAME(tkey, tvalue) xaml_map_view_2__##tkey##__##tvalue

    #define __XAML_MAP_VIEW_2_TYPE(tkey_name, tkey_interface, tvalue_name, tvalue_interface) \
        XAML_DECL_INTERFACE_T_(xaml_map_view_2, tkey_name##__##tvalue_name, XAML_MAP_VIEW_2_VTBL, tkey_name, tkey_interface, tvalue_name, tvalue_interface)
#endif // __cplusplus
#define XAML_MAP_VIEW_2_TYPE(tkey, tvalue) __XAML_MAP_VIEW_2_TYPE(tkey, tvalue)

XAML_TYPE_BASE(xaml_hasher_1, { 0xa7f9b6eb, 0xa71a, 0x4d5a, { 0x84, 0x54, 0x28, 0x83, 0x94, 0x1f, 0xb2, 0xb0 } })

#define XAML_HASHER_1_VTBL(type, TN, TI)            \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));      \
    XAML_METHOD(hash, type, TI, XAML_STD int32_t*); \
    XAML_METHOD(equals, type, TI, TI, bool*)

#ifdef __cplusplus
XAML_DECL_INTERFACE_T_(xaml_hasher, xaml_object, XAML_HASHER_1_VTBL)

    #define XAML_HASHER_1_NAME(type) xaml_hasher<type>

    #define __XAML_HASHER_1_TYPE(type) typedef xaml_hasher<type> xaml_hasher_1__##type;
#else
    #define XAML_HASHER_1_NAME(type) xaml_hasher_1__##type

    #define __XAML_HASHER_1_TYPE(type_name, type_interface) \
        XAML_DECL_INTERFACE_T_(xaml_hasher_1, type_name, XAML_HASHER_1_VTBL, type_name, type_interface)
#endif // __cplusplus
#define XAML_HASHER_1_TYPE(type) __XAML_HASHER_1_TYPE(type)

#ifndef xaml_hasher_1__xaml_string_defined
    #define xaml_hasher_1__xaml_string_defined
XAML_HASHER_1_TYPE(XAML_T_O(xaml_string))
#endif // !xaml_hasher_1__xaml_string_defined

EXTERN_C XAML_API xaml_result XAML_CALL xaml_hasher_string_default(XAML_HASHER_1_NAME(xaml_string) **) XAML_NOEXCEPT;

#ifdef __cplusplus
template <typename T>
struct __xaml_hasher_implement : xaml_implement<__xaml_hasher_implement<T>, xaml_hasher<T>>
{
    #ifdef XAML_SUPPORT_FUNCTION2
    using hash_func_type = fu2::unique_function<xaml_result(xaml_interface_t<T>, int32_t*) noexcept>;
    using eq_func_type = fu2::unique_function<xaml_result(xaml_interface_t<T>, xaml_interface_t<T>, bool*) noexcept>;
    #else
    using hash_func_type = std::function<xaml_result(xaml_interface_t<T>, int32_t*)>;
    using eq_func_type = std::function<xaml_result(xaml_interface_t<T>, xaml_interface_t<T>, bool*)>;
    #endif // XAML_SUPPORT_FUNCTION2

    hash_func_type m_func;
    eq_func_type m_eq_func;

    __xaml_hasher_implement(hash_func_type&& func, eq_func_type&& eq_func) noexcept
        : m_func(std::move(func)), m_eq_func(std::move(eq_func)) {}

    xaml_result XAML_CALL hash(xaml_interface_t<T> obj, int32_t* phash) noexcept override
    {
        return m_func(obj, phash);
    }

    xaml_result XAML_CALL equals(xaml_interface_t<T> lhs, xaml_interface_t<T> rhs, bool* pb) noexcept override
    {
        return m_eq_func(lhs, rhs, pb);
    }
};

    #ifdef XAML_SUPPORT_FUNCTION2
template <typename T>
xaml_result XAML_CALL xaml_hasher_new(fu2::unique_function<xaml_result(xaml_interface_t<T>, std::int32_t*) noexcept>&& hfunc, fu2::unique_function<xaml_result(xaml_interface_t<T>, xaml_interface_t<T>, bool*) noexcept>&& eqfunc, xaml_hasher<T>** ptr) noexcept
{
    return xaml_object_new<__xaml_hasher_implement<T>>(ptr, std::move(hfunc), std::move(eqfunc));
}
    #else
template <typename T>
xaml_result XAML_CALL xaml_hasher_new(std::function<xaml_result(xaml_interface_t<T>, std::int32_t*)>&& hfunc, std::function<xaml_result(xaml_interface_t<T>, xaml_interface_t<T>, bool*)>&& eqfunc, xaml_hasher<T>** ptr) noexcept
{
    return xaml_object_new<__xaml_hasher_implement<T>>(ptr, std::move(hfunc), std::move(eqfunc));
}
    #endif // XAML_SUPPORT_FUNCTION2

template <typename T>
xaml_result XAML_CALL xaml_hasher_new(xaml_hasher<T>** ptr) noexcept
{
    return xaml_hasher_new(
    #ifdef XAML_SUPPORT_FUNCTION2
        fu2::unique_function<xaml_result(xaml_interface_t<T>, std::int32_t*) noexcept> {
    #else
        std::function<xaml_result(xaml_interface_t<T>, std::int32_t*)>{
    #endif // XAML_SUPPORT_FUNCTION2
            [](xaml_interface_t<T> value, std::int32_t* phash) noexcept -> xaml_result {
                static std::hash<xaml_interface_var_t<T>> hasher{};
                std::size_t const std_hash = hasher(value);
                std::int32_t const* ptr = reinterpret_cast<std::int32_t const*>(&std_hash);
    #if SIZE_MAX == UINT64_MAX
                static_assert(sizeof(XAML_STD size_t) == sizeof(XAML_STD uint64_t), "Unknown 64-bit platform.");
                *phash = ptr[0] ^ ptr[1];
    #elif SIZE_MAX == UINT32_MAX
                static_assert(sizeof(XAML_STD size_t) == sizeof(XAML_STD uint32_t), "Unknown 32-bit platform.");
                *phash = ptr[0];
    #else
        #error Cannot determine platform architecture
    #endif
                return XAML_S_OK;
            } },
    #ifdef XAML_SUPPORT_FUNCTION2
        fu2::unique_function<xaml_result(xaml_interface_t<T>, xaml_interface_t<T>, bool*) noexcept> {
    #else
        std::function<xaml_result(xaml_interface_t<T>, xaml_interface_t<T>, bool*)>{
    #endif // XAML_SUPPORT_FUNCTION2
            [](xaml_interface_t<T> lvalue, xaml_interface_t<T> rvalue, bool* pb) noexcept -> xaml_result {
                *pb = lvalue == rvalue;
                return XAML_S_OK;
            } },
        ptr);
}

template <typename T>
struct __std_xaml_hasher
{
private:
    xaml_ptr<xaml_hasher<T>> m_inner;

public:
    __std_xaml_hasher() noexcept { XAML_ASSERT_SUCCEEDED(xaml_hasher_new<T>(&m_inner)); }
    __std_xaml_hasher(xaml_ptr<xaml_hasher<T>> const& hasher) noexcept : m_inner(hasher) {}

    xaml_ptr<xaml_hasher<T>> get_hasher() const noexcept { return m_inner; }

    std::size_t operator()(xaml_interface_t<T> obj) const noexcept
    {
        std::int32_t hash;
        XAML_ASSERT_SUCCEEDED(m_inner->hash(obj, &hash));
        return static_cast<std::size_t>(hash);
    }
};

template <typename T>
struct __std_xaml_eq
{
private:
    xaml_ptr<xaml_hasher<T>> m_inner;

public:
    __std_xaml_eq() noexcept { XAML_ASSERT_SUCCEEDED(xaml_hasher_new<T>(&m_inner)); }
    __std_xaml_eq(xaml_ptr<xaml_hasher<T>> const& hasher) noexcept : m_inner(hasher) {}

    xaml_ptr<xaml_hasher<T>> get_hasher() const noexcept { return m_inner; }

    bool operator()(xaml_interface_t<T> lhs, xaml_interface_t<T> rhs) const noexcept
    {
        bool eq;
        XAML_ASSERT_SUCCEEDED(m_inner->equals(lhs, rhs, &eq));
        return eq;
    }
};
#endif // __cplusplus

XAML_TYPE_BASE(xaml_map_2, { 0xe457ba53, 0x03d1, 0x4047, { 0xac, 0x85, 0x83, 0xdd, 0x85, 0x34, 0x6c, 0x7e } })

#define XAML_MAP_2_VTBL(type, TKeyN, TKeyI, TValueN, TValueI)                      \
    XAML_VTBL_INHERIT(XAML_MAP_VIEW_2_VTBL(type, TKeyN, TKeyI, TValueN, TValueI)); \
    XAML_METHOD(insert, type, TKeyI, TValueI, bool*);                              \
    XAML_METHOD(remove, type, TKeyI);                                              \
    XAML_METHOD(clear, type);                                                      \
    XAML_METHOD(get_hasher, type, XAML_HASHER_1_NAME(TKeyN)**)

#ifdef __cplusplus
template <typename TKey, typename TValue>
struct xaml_map : xaml_map_view<TKey, TValue>
{
    XAML_MAP_2_VTBL(xaml_map, TKey, xaml_interface_t<TKey>, TValue, xaml_interface_t<TValue>);
};

template <typename TKey, typename TValue>
struct xaml_base<xaml_map<TKey, TValue>>
{
    using type = xaml_map_view<TKey, TValue>;
};

template <typename TKey, typename TValue>
struct xaml_type_guid<xaml_map<TKey, TValue>>
{
    static constexpr xaml_guid value = xaml_guid_xaml_map_2;
};

    #define XAML_MAP_2_NAME(tkey, tvalue) xaml_map<tkey, tvalue>

    #define __XAML_MAP_2_TYPE(tkey, tvalue) typedef xaml_map<tkey, tvalue> xaml_map_2__##tkey##__##tvalue;
#else
    #define XAML_MAP_2_NAME(tkey, tvalue) xaml_map_2__##tkey##__##tvalue

    #define __XAML_MAP_2_TYPE(tkey_name, tkey_interface, tvalue_name, tvalue_interface) \
        XAML_DECL_INTERFACE_T_(xaml_map_2, tkey_name##__##tvalue_name, XAML_MAP_2_VTBL, tkey_name, tkey_interface, tvalue_name, tvalue_interface)
#endif // __cplusplus
#define XAML_MAP_2_TYPE(tkey, tvalue) __XAML_MAP_2_TYPE(tkey, tvalue)

#ifdef __cplusplus
template <typename TKey, typename TValue>
struct __xaml_map_enumerator_implement : xaml_implement<__xaml_map_enumerator_implement<TKey, TValue>, xaml_enumerator<xaml_key_value_pair<TKey, TValue>>>
{
    using inner_map_type = std::unordered_map<xaml_interface_var_t<TKey>, xaml_interface_var_t<TValue>, __std_xaml_hasher<TKey>, __std_xaml_eq<TKey>>;
    using iterator_type = typename inner_map_type::const_iterator;
    iterator_type m_begin, m_end;
    bool m_init;

    __xaml_map_enumerator_implement(iterator_type begin, iterator_type end) noexcept
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

    xaml_result XAML_CALL get_current(xaml_key_value_pair<TKey, TValue>** ptr) noexcept override
    {
        if (m_begin == m_end) return XAML_E_OUTOFBOUNDS;
        return xaml_key_value_pair_new<TKey, TValue>(m_begin->first, m_begin->second, ptr);
    }
};

template <typename TKey, typename TValue>
struct __xaml_map_implement : xaml_implement<__xaml_map_implement<TKey, TValue>, xaml_map<TKey, TValue>>
{
    using inner_map_type = std::unordered_map<xaml_interface_var_t<TKey>, xaml_interface_var_t<TValue>, __std_xaml_hasher<TKey>, __std_xaml_eq<TKey>>;

    inner_map_type m_map{};

    __xaml_map_implement() noexcept : m_map(1, __std_xaml_hasher<TKey>{}, __std_xaml_eq<TKey>{}) {}
    __xaml_map_implement(xaml_ptr<xaml_hasher<TKey>> const& hasher) noexcept : m_map(1, __std_xaml_hasher<TKey>{ hasher }, __std_xaml_eq<TKey>{ hasher }) {}

    xaml_result XAML_CALL get_size(int32_t* psize) noexcept override
    {
        *psize = (int32_t)m_map.size();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL lookup(xaml_interface_t<TKey> key, xaml_interface_t<TValue>* ptr) noexcept override
    {
        auto it = m_map.find(key);
        if (it == m_map.end()) return XAML_E_KEYNOTFOUND;
        return xaml_interface_assign<TValue>(it->second, ptr);
    }

    xaml_result XAML_CALL has_key(xaml_interface_t<TKey> key, bool* pb) noexcept override
    {
    #ifdef XAML_APPLE
        auto it = m_map.find(key);
        *pb = it != m_map.end();
    #else
        *pb = m_map.contains(key);
    #endif // XAML_APPLE
        return XAML_S_OK;
    }

    xaml_result XAML_CALL insert(xaml_interface_t<TKey> key, xaml_interface_t<TValue> value, bool* pb) noexcept override
    {
        auto [it, inserted] = m_map.emplace(key, value);
        if (!inserted)
        {
            it->second = value;
        }
        if (pb) *pb = !inserted;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL remove(xaml_interface_t<TKey> key) noexcept override
    {
        m_map.erase(key);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL clear() noexcept override
    {
        m_map.clear();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_hasher(xaml_hasher<TKey>** ptr) noexcept override
    {
        auto hasher = m_map.hash_function().get_hasher();
        return hasher->query(ptr);
    }

    xaml_result XAML_CALL get_enumerator(xaml_enumerator<xaml_key_value_pair<TKey, TValue>>** ptr) noexcept override
    {
        return xaml_object_new<__xaml_map_enumerator_implement<TKey, TValue>>(ptr, m_map.begin(), m_map.end());
    }
};

template <typename TKey, typename TValue>
xaml_result XAML_CALL xaml_map_new(xaml_map<TKey, TValue>** ptr) noexcept
{
    return xaml_object_new<__xaml_map_implement<TKey, TValue>>(ptr);
}

template <typename TKey, typename TValue>
xaml_result XAML_CALL xaml_map_new(xaml_hasher<TKey>* hasher, xaml_map<TKey, TValue>** ptr) noexcept
{
    return xaml_object_new<__xaml_map_implement<TKey, TValue>>(ptr, hasher);
}
#endif // __cplusplus

#endif // !XAML_MAP_H
