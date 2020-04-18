#ifndef XAML_MAP_H
#define XAML_MAP_H

#ifdef __cplusplus
#include <functional>
#include <unordered_map>
#include <xaml/box.h>
#include <xaml/xaml_ptr.hpp>
#else
#include <stdbool.h>
#endif // __cplusplus

#include <xaml/enumerable.h>

XAML_CLASS(xaml_key_value_pair, { 0x41ea97bb, 0xda95, 0x430c, { 0xa8, 0x1b, 0xdb, 0x06, 0x61, 0x9f, 0x7e, 0x96 } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_key_value_pair : xaml_object
{
    virtual xaml_result XAML_CALL get_key(xaml_object**) noexcept = 0;
    virtual xaml_result XAML_CALL get_value(xaml_object**) noexcept = 0;
};
#else
#define XAML_KEY_VALUE_PAIR_VTBL(type)                           \
    xaml_result(XAML_CALL* get_key)(type* const, xaml_object**); \
    xaml_result(XAML_CALL* get_value)(type* const, xaml_object**);

struct xaml_key_value_pair
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_key_value_pair)
        XAML_KEY_VALUE_PAIR_VTBL(xaml_key_value_pair)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_API xaml_result xaml_key_value_pair_new(xaml_object*, xaml_object*, xaml_key_value_pair**) XAML_NOEXCEPT;

XAML_CLASS(xaml_map_view, { 0x15549c22, 0x40d1, 0x4af1, { 0xad, 0x81, 0x0d, 0xd3, 0xda, 0x1a, 0x87, 0xba } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_map_view : xaml_enumerable
{
    virtual xaml_result XAML_CALL lookup(xaml_object*, xaml_object**) noexcept = 0;
    virtual xaml_result XAML_CALL has_key(xaml_object*, bool*) noexcept = 0;
    virtual xaml_result XAML_CALL get_size(std::size_t*) noexcept = 0;
};
#else
#define XAML_MAP_VIEW_VTBL(type)                                              \
    xaml_result(XAML_CALL* lookup)(type* const, xaml_object*, xaml_object**); \
    xaml_result(XAML_CALL* has_key)(type* const, xaml_object*, bool*);        \
    xaml_result(XAML_CALL* get_size)(type* const, size_t*);

struct xaml_map_view
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_map_view)
        XAML_ENUMERABLE_VTBL(xaml_map_view)
        XAML_MAP_VIEW_VTBL(xaml_map_view)
    } const* vtbl;
};
#endif // __cplusplus

XAML_CLASS(xaml_hasher, { 0xa7f9b6eb, 0xa71a, 0x4d5a, { 0x84, 0x54, 0x28, 0x83, 0x94, 0x1f, 0xb2, 0xb0 } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_hasher : xaml_object
{
    virtual xaml_result XAML_CALL hash(xaml_object*, std::size_t*) noexcept = 0;
    virtual xaml_result XAML_CALL equal(xaml_object*, xaml_object*, bool*) noexcept = 0;
};
#else
#define XAML_COMPARER_VTBL(type)                                      \
    xaml_result(XAML_CALL* hash)(type* const, xaml_object*, size_t*); \
    xaml_result(XAML_CALL* equal)(type* const, xaml_object*, xaml_object*, bool*);

struct xaml_hasher
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_hasher)
        XAML_COMPARER_VTBL(xaml_hasher);
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_API xaml_result xaml_hasher_default(xaml_hasher**) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result xaml_hasher_new(xaml_result (*)(xaml_object*, XAML_CSTD size_t*), xaml_result (*)(xaml_object*, xaml_object*, bool*), xaml_hasher**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_API xaml_result xaml_hasher_new(std::function<xaml_result(xaml_object*, std::size_t*)>&&, std::function<xaml_result(xaml_object*, xaml_object*, bool*)>&&, xaml_hasher**) noexcept;

template <typename T>
inline xaml_result xaml_hasher_new(xaml_hasher** ptr) noexcept
{
    return xaml_hasher_new(
        std::function<xaml_result(xaml_object*, std::size_t*)>{
            [](xaml_object* obj, std::size_t* phash) -> xaml_result {
                static std::hash<T> hasher{};
                T value;
                XAML_RETURN_IF_FAILED(unbox_value(obj, value));
                *phash = hasher(value);
                return XAML_S_OK;
            } },
        std::function<xaml_result(xaml_object*, xaml_object*, bool*)>{
            [](xaml_object* lhs, xaml_object* rhs, bool* pb) -> xaml_result {
                T lvalue, rvalue;
                XAML_RETURN_IF_FAILED(unbox_value(lhs, lvalue));
                XAML_RETURN_IF_FAILED(unbox_value(rhs, rvalue));
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
    __std_xaml_hasher() { XAML_THROW_IF_FAILED(xaml_hasher_default(&m_inner)); }
    __std_xaml_hasher(xaml_ptr<xaml_hasher> const& hasher) : m_inner(std::move(hasher)) {}

    xaml_ptr<xaml_hasher> get_hasher() const noexcept { return m_inner; }

    std::size_t operator()(xaml_ptr<xaml_object> const& obj) const
    {
        std::size_t hash;
        XAML_THROW_IF_FAILED(m_inner->hash(obj.get(), &hash));
        return hash;
    }
};

struct __std_xaml_eq
{
private:
    xaml_ptr<xaml_hasher> m_inner;

public:
    __std_xaml_eq() { XAML_THROW_IF_FAILED(xaml_hasher_default(&m_inner)); }
    __std_xaml_eq(xaml_ptr<xaml_hasher> const& hasher) : m_inner(std::move(hasher)) {}

    xaml_ptr<xaml_hasher> get_hasher() const noexcept { return m_inner; }

    bool operator()(xaml_ptr<xaml_object> const& lhs, xaml_ptr<xaml_object> const& rhs) const
    {
        bool eq;
        XAML_THROW_IF_FAILED(m_inner->equal(lhs.get(), rhs.get(), &eq));
        return eq;
    }
};
#endif // __cplusplus

XAML_CLASS(xaml_map, { 0xe457ba53, 0x03d1, 0x4047, { 0xac, 0x85, 0x83, 0xdd, 0x85, 0x34, 0x6c, 0x7e } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_map : xaml_map_view
{
    virtual xaml_result XAML_CALL insert(xaml_object*, xaml_object*, bool*) noexcept = 0;
    virtual xaml_result XAML_CALL remove(xaml_object*) noexcept = 0;
    virtual xaml_result XAML_CALL clear() noexcept = 0;
    virtual xaml_result XAML_CALL get_hasher(xaml_hasher**) noexcept = 0;
};
#else
#define XAML_MAP_VTBL(type)                                                         \
    xaml_result(XAML_CALL* insert)(type* const, xaml_object*, xaml_object*, bool*); \
    xaml_result(XAML_CALL* remove)(type* const, xaml_object*);                      \
    xaml_result(XAML_CALL* clear)(type* const);                                     \
    xaml_result(XAML_CALL* get_hasher)(type* const, xaml_hasher**);

struct xaml_map
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_map)
        XAML_ENUMERABLE_VTBL(xaml_map)
        XAML_MAP_VIEW_VTBL(xaml_map)
        XAML_MAP_VTBL(xaml_map)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_API xaml_result xaml_map_new(xaml_map**) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result xaml_map_new_with_hasher(xaml_hasher*, xaml_map**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_API xaml_result xaml_map_new(std::unordered_map<xaml_ptr<xaml_object>, xaml_ptr<xaml_object>, __std_xaml_hasher, __std_xaml_eq>&&, xaml_map**) noexcept;
#endif // __cplusplus

#endif // !XAML_MAP_H
