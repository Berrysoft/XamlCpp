#ifndef XAML_MAP_H
#define XAML_MAP_H

#ifdef __cplusplus
    #include <functional>
    #include <unordered_map>
    #include <xaml/box.h>
    #include <xaml/ptr.hpp>
#else
    #include <stdbool.h>
#endif // __cplusplus

#include <xaml/enumerable.h>

__XAML_TYPE_NAME_BASE(xaml_key_value_pair, { 0x41ea97bb, 0xda95, 0x430c, { 0xa8, 0x1b, 0xdb, 0x06, 0x61, 0x9f, 0x7e, 0x96 } })

#define XAML_KEY_VALUE_PAIR_T_T_VTBL(type, TKeyN, TKeyI, TValueN, TValueI) \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                             \
    XAML_METHOD(get_key, type, TKeyI*);                                    \
    XAML_METHOD(get_value, type, TValueI*)

#ifdef __cplusplus
template <typename TKey, typename TValue>
struct xaml_key_value_pair : xaml_object
{
    XAML_KEY_VALUE_PAIR_T_T_VTBL(xaml_key_value_pair, TKey, xaml_interface_t<TKey>, TValue, xaml_interface_t<TValue>);
};

template <typename TKey, typename TValue>
struct xaml_base<xaml_key_value_pair<TKey, TValue>>
{
    using type = xaml_object;
};

template <typename TKey, typename TValue>
struct xaml_type_guid<xaml_key_value_pair<TKey, TValue>>
{
    static constexpr xaml_guid value = xaml_guid_xaml_key_value_pair;
};

    #define XAML_KEY_VALUE_PAIR_T_T_NAME(tkey, tvalue) xaml_key_value_pair<tkey, tvalue>

    #define XAML_KEY_VALUE_PAIR_T_T_TYPE(tkey, tkey_interface, tvalue, tvalue_interface) typedef xaml_key_value_pair<tkey, tvalue> xaml_key_value_pair__##tkey##____##tvalue##__;
#else
    #define XAML_KEY_VALUE_PAIR_T_T_NAME(tkey, tvalue) xaml_key_value_pair__##tkey##____##tvalue##__

    #define XAML_KEY_VALUE_PAIR_T_T_TYPE(tkey_name, tkey_interface, tvalue_name, tvalue_interface) \
        XAML_DECL_INTERFACE_T_(xaml_key_value_pair, tkey_name##____##tvalue_name, XAML_KEY_VALUE_PAIR_T_T_VTBL, tkey_name, tkey_interface, tvalue_name, tvalue_interface)
#endif // __cplusplus

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

XAML_CLASS(xaml_map_view, { 0x15549c22, 0x40d1, 0x4af1, { 0xad, 0x81, 0x0d, 0xd3, 0xda, 0x1a, 0x87, 0xba } })

#define XAML_MAP_VIEW_VTBL(type)                            \
    XAML_VTBL_INHERIT(XAML_ENUMERABLE_VTBL(type));          \
    XAML_METHOD(lookup, type, xaml_object*, xaml_object**); \
    XAML_METHOD(has_key, type, xaml_object*, bool*);        \
    XAML_METHOD(get_size, type, XAML_STD int32_t*)

XAML_DECL_INTERFACE_(xaml_map_view, xaml_enumerable)
{
    XAML_DECL_VTBL(xaml_map_view, XAML_MAP_VIEW_VTBL);
};

XAML_CLASS(xaml_hasher, { 0xa7f9b6eb, 0xa71a, 0x4d5a, { 0x84, 0x54, 0x28, 0x83, 0x94, 0x1f, 0xb2, 0xb0 } })

#define XAML_HASHER_VTBL(type)                                \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                \
    XAML_METHOD(hash, type, xaml_object*, XAML_STD int32_t*); \
    XAML_METHOD(equals, type, xaml_object*, xaml_object*, bool*)

XAML_DECL_INTERFACE_(xaml_hasher, xaml_object)
{
    XAML_DECL_VTBL(xaml_hasher, XAML_HASHER_VTBL);
};

EXTERN_C XAML_API xaml_result XAML_CALL xaml_hasher_default(xaml_hasher**) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result XAML_CALL xaml_hasher_string_default(xaml_hasher**) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result XAML_CALL xaml_hasher_new(xaml_result (*)(xaml_object*, XAML_STD int32_t*), xaml_result (*)(xaml_object*, xaml_object*, bool*), xaml_hasher**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_API xaml_result XAML_CALL xaml_hasher_new(std::function<xaml_result(xaml_object*, std::int32_t*)>&&, std::function<xaml_result(xaml_object*, xaml_object*, bool*)>&&, xaml_hasher**) noexcept;

template <typename T>
inline xaml_result XAML_CALL xaml_hasher_new(xaml_hasher** ptr) noexcept
{
    return xaml_hasher_new(
        std::function<xaml_result(xaml_object*, std::int32_t*)>{
            [](xaml_object* obj, std::int32_t* phash) noexcept -> xaml_result {
                static std::hash<T> hasher{};
                T value;
                XAML_RETURN_IF_FAILED(xaml_unbox_value(obj, &value));
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
        std::function<xaml_result(xaml_object*, xaml_object*, bool*)>{
            [](xaml_object* lhs, xaml_object* rhs, bool* pb) noexcept -> xaml_result {
                T lvalue, rvalue;
                XAML_RETURN_IF_FAILED(xaml_unbox_value(lhs, &lvalue));
                XAML_RETURN_IF_FAILED(xaml_unbox_value(rhs, &rvalue));
                *pb = lvalue == rvalue;
                return XAML_S_OK;
            } },
        ptr);
}

struct __std_xaml_hasher
{
private:
    xaml_ptr<xaml_hasher> m_inner;

public:
    __std_xaml_hasher() noexcept { XAML_ASSERT_SUCCEEDED(xaml_hasher_default(&m_inner)); }
    __std_xaml_hasher(xaml_ptr<xaml_hasher> const& hasher) noexcept : m_inner(hasher) {}

    xaml_ptr<xaml_hasher> get_hasher() const noexcept { return m_inner; }

    std::size_t operator()(xaml_ptr<xaml_object> const& obj) const noexcept
    {
        std::int32_t hash;
        XAML_ASSERT_SUCCEEDED(m_inner->hash(obj, &hash));
        return (std::size_t)hash;
    }
};

struct __std_xaml_eq
{
private:
    xaml_ptr<xaml_hasher> m_inner;

public:
    __std_xaml_eq() noexcept { XAML_ASSERT_SUCCEEDED(xaml_hasher_default(&m_inner)); }
    __std_xaml_eq(xaml_ptr<xaml_hasher> const& hasher) noexcept : m_inner(hasher) {}

    xaml_ptr<xaml_hasher> get_hasher() const noexcept { return m_inner; }

    bool operator()(xaml_ptr<xaml_object> const& lhs, xaml_ptr<xaml_object> const& rhs) const noexcept
    {
        bool eq;
        XAML_ASSERT_SUCCEEDED(m_inner->equals(lhs, rhs, &eq));
        return eq;
    }
};
#endif // __cplusplus

XAML_CLASS(xaml_map, { 0xe457ba53, 0x03d1, 0x4047, { 0xac, 0x85, 0x83, 0xdd, 0x85, 0x34, 0x6c, 0x7e } })

#define XAML_MAP_VTBL(type)                                       \
    XAML_VTBL_INHERIT(XAML_MAP_VIEW_VTBL(type));                  \
    XAML_METHOD(insert, type, xaml_object*, xaml_object*, bool*); \
    XAML_METHOD(remove, type, xaml_object*);                      \
    XAML_METHOD(clear, type);                                     \
    XAML_METHOD(get_hasher, type, xaml_hasher**)

XAML_DECL_INTERFACE_(xaml_map, xaml_map_view)
{
    XAML_DECL_VTBL(xaml_map, XAML_MAP_VTBL);
};

EXTERN_C XAML_API xaml_result XAML_CALL xaml_map_new(xaml_map**) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result XAML_CALL xaml_map_new_with_hasher(xaml_hasher*, xaml_map**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_API xaml_result XAML_CALL xaml_map_new(std::unordered_map<xaml_ptr<xaml_object>, xaml_ptr<xaml_object>, __std_xaml_hasher, __std_xaml_eq>&&, xaml_map**) noexcept;
#endif // __cplusplus

#endif // !XAML_MAP_H
